#include "node_rakett.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

int RocketLoop()
{   
    std::queue<can_frame> canRxfifo;

    MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000); //CAN Bus interface
    if (canbus.reset() != MCP2515::ERROR_OK)                                {printf("CAN reset failed!\n");         return 1;}
    if (canbus.setBitrate(CAN_1000KBPS, MCP_16MHZ) != MCP2515::ERROR_OK)    {printf("Setting bitrate failed!\n");   return 2;}
    canbus.setNormalMode();

    message flerecan(1);

    //Main loop
    while(true)
    {

        processCanbusMessage(canbus, canRxfifo);
        if(!canRxfifo.empty()){
            can_frame DenneMeldingenHerKanDuSendeRuben = canRxfifo.front(); canRxfifo.pop();
            if(flerecan.can2message(DenneMeldingenHerKanDuSendeRuben)){
                flerecan.send();
            }
        }

    }
}

/*uint8_t data[4+8];
            memcpy(data, &rx.can_id, sizeof(rx.can_id));
            memcpy(data+4, &rx.data, sizeof(rx.data));
            message melding(1, sizeof(data), data);

            melding.send();*/


bool IDisOfInterest(const can_frame incoming)
{
    uint32_t id = incoming.can_id;
    for (uint32_t i = 0; i < sizeof(idsToSendToGround); i++)
    {
        if (id == idsToSendToGround[i]) {return true;}
    }
    return false;
}


void processCanbusMessage(MCP2515& canbus, std::queue<can_frame>& canRxfifo)
{
    struct can_frame canrx;
    if (canbus.readMessage(&canrx) != MCP2515::ERROR_OK)            {return;}
    printf("Received message from ID: 0x%03X\n", canrx.can_id);
    if (!IDisOfInterest(canrx))                                     {return;}
    if (canRxfifo.size() >= MaxBufferSize)                          {canRxfifo.pop();}
    canRxfifo.push(canrx);
}