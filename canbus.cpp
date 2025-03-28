#include "canbus.hpp"



void canbusInit()
{
    
    gpio_init(Pin_Can_Cs);
    gpio_set_dir(Pin_Can_Cs, GPIO_OUT);
    gpio_put(Pin_Can_Cs, 1);

    gpio_init(Pin_Can_RESET );
    gpio_set_dir(Pin_Can_RESET , GPIO_OUT);
    gpio_put(Pin_Can_RESET , 1);

    gpio_init(Pin_Can_STBY);
    gpio_set_dir(Pin_Can_STBY, GPIO_IN);
    gpio_pull_down(Pin_Can_STBY);

    
    gpio_put(Pin_Can_RESET , 0);
    sleep_ms(100);
    gpio_put(Pin_Can_RESET , 1);
    sleep_ms(2);
    

    if (canbus.reset() != MCP2515::ERROR_OK)                                {while (true){printf("CAN reset failed!\n");}}
    if (canbus.setBitrate(CAN_1000KBPS, MCP_20MHZ) != MCP2515::ERROR_OK)    {while (true){printf("Setting bitrate failed!\n");}}
    canbus.setNormalMode();
}

bool IDisOfInterest(const can_frame incoming)
{
    uint32_t id = incoming.can_id;
    for (uint32_t i = 0; i < sizeof(idsToSendToGround); i++)
    {
        if (id == idsToSendToGround[i]) {return true;}
    }
    return false;
}


bool processCanbusMessageRx()
{
    struct can_frame canrx;
    MCP2515::ERROR error = canbus.readMessage(&canrx);
    //printf("ERROR: %d\n", error);
    if (error != MCP2515::ERROR_OK)           {return false;}
    printf("Received message from ID: 0x%03X\n", canrx.can_id);
    if (!IDisOfInterest(canrx))                                 {return false;}
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(canrx);
    return true;
}


bool processCanbusMessageTx()
{
    if (canTxfifo.empty())                                              {return false;}
    can_frame canTx = canTxfifo.front();                                canTxfifo.pop();
    if (canbus.sendMessage(&canTx) != MCP2515::ERROR_OK)                   {return false;}
    return true;
}


bool retriveNextCanFrame(can_frame &frameToBeRecieved)
{
    if(!canRxfifo.empty())
    {
        frameToBeRecieved = canRxfifo.front();
        canRxfifo.pop();
        return true;
    }
    else {return false;}
}


bool sendCanFrame(can_frame &frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize)  {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    return true;
}


bool loopbackCanFrame(can_frame &frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize)  {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(frameToBeSent);
    return true;
}
