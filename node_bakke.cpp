#include "mcp2515.h"
#include "node_bakke.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

int GroundLoop()
{
    message mota;
    
    while (1)
    {
        if(LoRa.parsePacket() != 0){
            mota.receive();
            int NumberOfCan = mota.length/(CanIdLength + CanDataLength);
            can_frame CanMessages[NumberOfCan];
            for (int i = NumberOfCan-1; i > -1; i--){
                CanMessages[i] = mota.message2can();
            }
            for (int i = 0; i < NumberOfCan; i++){
                mota.printCan(CanMessages[i]);
            }   
          }
    }
    return 0;
}