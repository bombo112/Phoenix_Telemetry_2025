#include <queue>
#include <stdio.h>
#include "mcp2515.h"
#include "Settings.hpp"

#ifndef CANBUS_HPP
#define CANBUS_HPP

class canbusInterface
{
private:
    std::queue<can_frame> canRxfifo;
    std::queue<can_frame> canTxfifo;
    MCP2515 bus; //CAN Bus interface

    bool processCanbusMessageRx();
    bool processCanbusMessageTx();
    bool IDisOfInterest(const can_frame incoming);

public:
    bool retriveNextCanFrame(can_frame &frameToBeRecieved);
    bool sendCanFrame(can_frame &frameToBeSent);
    bool loopbackCanFrame(can_frame &frameToBeSent);        // Send the can frame out on bus and loops it back on the recieving end to be read back to ground

    canbusInterface();
};











#endif