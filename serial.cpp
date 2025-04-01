#include "serial.hpp"





void serialInit()
{
    stdio_set_chars_available_callback(serialReadHandler, NULL);
}


void serialReadHandler(void* param) {
    (void)param; // dump parameter

    int32_t receivedCharacter;
    while ((receivedCharacter = stdio_getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT)
    {   
        putchar_raw(receivedCharacter);
        if (receivedCharacter != '\n' && receivedCharacter != '\r')
        {
            inputStringSerial.push_back(static_cast<char>(receivedCharacter));
        }
        else
        {
            std::vector<std::string> sentenceParts = split(inputStringSerial, ':');
            if (sentenceParts.size() > 9){
                inputStringSerial.clear();
                continue;
            }
            canFrame NewCan;
            NewCan.id = stoi(sentenceParts[0], 0, 10);
        
            for (size_t i = 1; i < sentenceParts.size(); i++)   {NewCan.data[i-1] = stoi(sentenceParts[i], 0, 10);}
            if (serialRxfifo.size() >= MaxBufferSize) {serialRxfifo.pop();}

            serialRxfifo.push(NewCan);
            inputStringSerial.clear();
            NewCan.print();
            continue;
        }
    }
}

bool retriveNextSerialFrame(canFrame &frameToBeRecieved)
{
    if(!serialRxfifo.empty())
    {
        frameToBeRecieved = serialRxfifo.front();
        serialRxfifo.pop();
        return true;
    }
    else {return false;}
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
