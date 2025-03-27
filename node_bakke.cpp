#include "node_bakke.hpp"


int GroundLoop()
{
    message mota;
    can_frame MotatCan;
    while(1)
    {
        if(LoRa.parsePacket() != 0){
            mota.receive();
            int NumberOfCan = mota.length/(CanIdLength + CanDataLength);
            can_frame CanMessages[NumberOfCan];
            for (int i = NumberOfCan-1; i > -1; i--){
                CanMessages[i] = mota.MessageToCan();
            }
            for (int i = 0; i < NumberOfCan; i++){
                PrintCan(CanMessages[i]);
            }   
        }
        UsbToCan();
    }
    return 0;
}

void PrintCan(can_frame can){
    printf("%d", can.can_id);
    for(int i=0; i< 8; i++){
        printf(":%d",can.data[i]);
    }
    printf("\n");
}

void UsbToCan()
{
    static std::string sentence;

    int32_t receivedCharacter = getchar_timeout_us(10000);  // 10ms timeout

    if (receivedCharacter == PICO_ERROR_TIMEOUT) {return;}
    if (receivedCharacter != '\n' && receivedCharacter != '\r')
    {
        sentence.push_back((char)receivedCharacter);
        return;
    }

    std::vector<std::string> sentenceParts = split(sentence, ':');

    if (sentenceParts.size() > 9)
    {
        sentence.clear();
        return;
    }

    can_frame NewCan;
    NewCan.can_id = stoi(sentenceParts[0], 0, 10);


    for (size_t i = 1; i < sentenceParts.size(); i++)
    {
        NewCan.data[i-1] = stoi(sentenceParts[i], 0, 10);
    }
    
    PrintCan(NewCan);   //debug
}


std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}
