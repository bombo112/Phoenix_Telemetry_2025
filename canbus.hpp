#include <queue>
#include <stdio.h>
#include "mcp2515.h"
#include "Settings.hpp"

#ifndef CANBUS_HPP
#define CANBUS_HPP


inline std::queue<can_frame> canRxfifo;
inline std::queue<can_frame> canTxfifo;
inline MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000);

bool processCanbusMessageRx();
bool processCanbusMessageTx();
bool IDisOfInterest(const can_frame incoming);

bool retriveNextCanFrame(can_frame &frameToBeRecieved);
bool sendCanFrame(can_frame &frameToBeSent);
bool loopbackCanFrame(can_frame &frameToBeSent);        // Send the can frame out on bus and loops it back on the recieving end to be read back to ground

void canbusInit();




#endif