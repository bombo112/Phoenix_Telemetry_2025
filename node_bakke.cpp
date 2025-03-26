#include "node_bakke.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

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
                CanMessages[i] = mota.message2can();
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

void UsbToCan(){
    static std::queue<int> UsbFifo;
    int32_t NewUsbCharacter = getchar_timeout_us(10000);  // 10ms timeout
    if (NewUsbCharacter == PICO_ERROR_TIMEOUT) {return;}
    if (NewUsbCharacter != '\n'){
        if (UsbFifo.size() >= MaxBufferSize){UsbFifo.pop();}
        UsbFifo.push(NewUsbCharacter); 
        return;
    }
    int FifoLength = UsbFifo.size();
    char sentence[FifoLength];
    int dataLength = 1;
    for(int i = 0; i< FifoLength; i++){
        if(UsbFifo.front() ==':'){dataLength++;}
        sentence[i] = UsbFifo.front();
        UsbFifo.pop();
    }
    if(dataLength != (1+CanDataLength)){return;}

    can_frame NewCan;
    std::vector<std::string> sentence_elements = split(sentence, ':');
    NewCan.can_id = stoi(sentence_elements[0], 0, 10);
    for (size_t i = 1; i < dataLength; i++){
        NewCan.data[i-1] = stoi(sentence_elements[i], 0, 10);
    }
    
    //test
    PrintCan(NewCan);
}


std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}
