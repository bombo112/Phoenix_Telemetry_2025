#include "serial.hpp"


void serialInit()
{
    stdio_set_chars_available_callback(processSerialMessageRx, NULL);
}


void processSerialMessageRx(void* param)//må skru av interupts
{
    (void)param; // dump parameter

    int32_t receivedCharacter;
    while ((receivedCharacter = stdio_getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT)
    {   
        putchar_raw(receivedCharacter);//debug
        if (receivedCharacter != '\n' && receivedCharacter != '\r')
        {
            inputStringSerial.push_back(static_cast<char>(receivedCharacter));
        }
        else
        {
            std::vector<std::string> sentenceParts = split(inputStringSerial, ',');
            if (sentenceParts.size() > 9){
                inputStringSerial.clear();
                continue;
            }
            canFrame frame;
            frame.id = stoi(sentenceParts[0], 0, 10);
        
            for (size_t i = 1; i < sentenceParts.size(); i++)   {frame.data[i-1] = stoi(sentenceParts[i], 0, 16);}
            if (serialRxfifo.size() >= MaxBufferSize) {serialRxfifo.pop();}

            CheckIfResetGroundModule(frame);
            serialRxfifo.push(frame);
            inputStringSerial.clear();
            //frame.print(); //debug
            continue;
        }
    }
}


void processSerialMessageTx()
{
    while (!serialTxfifo.empty())
    {
        constexpr uint64_t UsHour = 3600000000;
        constexpr uint64_t UsMinute = 60000000;
        constexpr uint64_t UsSecond  = 1000000; 

        canFrame frame = serialTxfifo.front(); serialTxfifo.pop();
        uint64_t UsTime = parseTimeStamp(frame.time);

        uint64_t hours   = UsTime / UsHour;
        UsTime %= UsHour;

        uint64_t minutes = UsTime / UsMinute;
        UsTime %= UsMinute;

        uint64_t seconds = UsTime / UsSecond;
        uint64_t micro   = UsTime % UsSecond;

        char buffer[32];
        snprintf(buffer, 32, "%02llu:%02llu:%02llu.%06llu", hours, minutes, seconds, micro);

        printf("%d,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%s\n",
            frame.id,
            frame.data[0], 
            frame.data[1], 
            frame.data[2], 
            frame.data[3], 
            frame.data[4], 
            frame.data[5], 
            frame.data[6], 
            frame.data[7],
            buffer);
    }
}


std::vector<std::string> split(const std::string &s, const char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t pos = s.find(delimiter);
    while (pos != std::string::npos)
    {
        tokens.push_back(s.substr(start, pos - start));
        start = pos + 1;
        pos = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}


void sendFrameToSerial(canFrame frameToBeSent)
{
    if(frameToBeSent.id == InternalTelemetryMessageId){return;}
    if (serialTxfifo.size() >= MaxBufferSize) 
    {
        serialTxfifo.pop();
    }
    serialTxfifo.push(frameToBeSent);
}


canFrame retrieveFrameFromSerial()
{
    canFrame frameToBeRecieved;
    if(!serialRxfifo.empty())
    {
        frameToBeRecieved = serialRxfifo.front();
        serialRxfifo.pop();
        return frameToBeRecieved;
    }
    else
    {
        return frameToBeRecieved;
    }
}

void CheckIfResetGroundModule(canFrame FrameToCheck){
    if(FrameToCheck.id != ResetGroundModuleId)          {return;}
    pico_unique_board_id_t currentBoard;
    pico_get_unique_board_id(&currentBoard);
    if ((memcmp(currentBoard.id, GroundNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0) && FrameToCheck.CompareCanFrameDataToArray(ResetGroundModuleKey)){
        rom_reboot(BOOT_TYPE_NORMAL, 10,0,0);
        sleep_ms(20);
    }
}
