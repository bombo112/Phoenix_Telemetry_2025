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
            mota.print();
          }
          printf("bakkenode!");
    }
    return 0;
}