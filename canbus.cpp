#include "canbus.hpp"

canbusInterface::canbusInterface() : bus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000)
{
    if (bus.reset() != MCP2515::ERROR_OK)                                {while (true){printf("CAN reset failed!\n");}}
    if (bus.setBitrate(CAN_1000KBPS, MCP_16MHZ) != MCP2515::ERROR_OK)    {while (true){printf("Setting bitrate failed!\n");}}
    bus.setNormalMode();
}

bool canbusInterface::IDisOfInterest(const can_frame incoming)
{
    uint32_t id = incoming.can_id;
    for (uint32_t i = 0; i < sizeof(idsToSendToGround); i++)
    {
        if (id == idsToSendToGround[i]) {return true;}
    }
    return false;
}


bool canbusInterface::processCanbusMessageRx()
{
    struct can_frame canrx;
    if (bus.readMessage(&canrx) != MCP2515::ERROR_OK)           {return false;}
    printf("Received message from ID: 0x%03X\n", canrx.can_id);
    if (!IDisOfInterest(canrx))                                 {return false;}
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(canrx);
    return true;
}


bool canbusInterface::processCanbusMessageTx()
{
    if (canTxfifo.empty())                                              {return false;}
    can_frame canTx = canTxfifo.front();                                canTxfifo.pop();
    if (bus.sendMessage(&canTx) != MCP2515::ERROR_OK)                   {return false;}
    return true;
}


bool canbusInterface::retriveNextCanFrame(can_frame &frameToBeRecieved)
{
    if(!canRxfifo.empty())
    {
        frameToBeRecieved = canRxfifo.front();
        canRxfifo.pop();
        return true;
    }
    else {return false;}
}


bool canbusInterface::sendCanFrame(can_frame &frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize)  {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    return true;
}


bool canbusInterface::loopbackCanFrame(can_frame &frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize)  {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(frameToBeSent);
    return true;
}
