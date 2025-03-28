#include "node_bakke.hpp"
#include "serial.hpp"


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
                printCan(CanMessages[i]);
            }   
        }
        serialReadCAN();
    }
    return 0;
}


void printCan(can_frame message)
{
    printf("%d", message.can_id);
    for (size_t i = 0; i < 8; i++)
    {
        printf(":%d", message.data[i]);
    }
    printf("\n");
}