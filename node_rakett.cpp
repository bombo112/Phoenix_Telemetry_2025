#include "node_rakett.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

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

            uint8_t data[4+8];
            memcpy(data, &rx.can_id, sizeof(rx.can_id));
            memcpy(data+4, &rx.data, sizeof(rx.data));
            message melding(1, sizeof(data), data);

            melding.send();

            canRxfifo.push(rx);
        }
    }
}


