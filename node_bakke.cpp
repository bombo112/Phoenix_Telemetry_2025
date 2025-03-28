#include "node_bakke.hpp"
#include "serial.hpp"


int GroundLoop()
{
    
    
    message mota;
    canFrame MotatCan;
    while(1)
    {
        if(LoRa.parsePacket() != 0){
            mota.receive();
            int NumberOfCan = mota.length/(CanIdLength + CanDataLength);
            canFrame CanMessages[NumberOfCan];
            for (int i = NumberOfCan-1; i > -1; i--){
                CanMessages[i] = mota.MessageToCan();
            }
            for (int i = 0; i < NumberOfCan; i++){
                CanMessages[i].print();
            }   
        }
        serialReadCAN();
    }
    return 0;
}
