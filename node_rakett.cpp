#include "node_rakett.hpp"

int RocketLoop()
{
    MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000); //CAN Bus interface
    if (canbus.reset() != MCP2515::ERROR_OK)                                {printf("CAN reset failed!\n");         return 1;}
    if (canbus.setBitrate(CAN_1000KBPS, MCP_16MHZ) != MCP2515::ERROR_OK)    {printf("Setting bitrate failed!\n");   return 2;}
    canbus.setNormalMode();

    std::queue<can_frame> canRxfifo;




    //Main loop
    while(true)
    {
        struct can_frame rx;
        bool CAN;
        if (canbus.readMessage(&rx) == MCP2515::ERROR_OK)   {
            printf("Received message from ID: 0x%03X\n", rx.can_id);

            canRxfifo.push(rx);
        }



    }
}


