#include "node_rakett.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

int RocketLoop()
{   
    std::queue<can_frame> canRxfifo;
    std::queue<can_frame> canTxfifo;

    MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000); //CAN Bus interface
    if (canbus.reset() != MCP2515::ERROR_OK)                                {printf("CAN reset failed!\n");         return 1;}
    if (canbus.setBitrate(CAN_1000KBPS, MCP_16MHZ) != MCP2515::ERROR_OK)    {printf("Setting bitrate failed!\n");   return 2;}
    canbus.setNormalMode();

    //Main loop
    while(true)
    {
        processCanbusMessageRx(canbus, canRxfifo);
        processCanbusMessageTx(canbus, canTxfifo);
        can_frame DenneMeldingenHerKanDuSendeRuben = canRxfifo.front(); canRxfifo.pop();
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


bool processCanbusMessageRx(MCP2515& canbus, std::queue<can_frame>& fifo)
{
    struct can_frame canrx;
    if (canbus.readMessage(&canrx) != MCP2515::ERROR_OK)            {return false;}
    printf("Received message from ID: 0x%03X\n", canrx.can_id);
    if (!IDisOfInterest(canrx))                                     {return false;}
    if (fifo.size() >= MaxBufferSize)                          {fifo.pop();}
    fifo.push(canrx);
    return true;
}

bool processCanbusMessageTx(MCP2515& canbus, std::queue<can_frame>& fifo)
{
    if (fifo.empty())                                          {return false;}
    can_frame canTx = fifo.front();                            fifo.pop();
    if (canbus.sendMessage(&canTx) != MCP2515::ERROR_OK)            {return false;}
    return true;
}

