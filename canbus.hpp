#include <queue>
#include <cstdio>
#include "mcp2515.h"
#include "Settings.hpp"
#include "pico/time.h"
#include "pico/bootrom.h"
#include "pico/bootrom_constants.h"

#ifndef CANBUS_HPP
#define CANBUS_HPP


class canFrame
{
public:
    uint16_t id;
    uint16_t delta;
    uint8_t data[8];

    void print();
    canFrame(can_frame frameToConvert);
    can_frame convert();
    canFrame();
};

inline std::queue<canFrame> canRxfifo;
inline std::queue<canFrame> canTxfifo;
inline MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000);

bool processCanbusMessageRx();
bool processCanbusMessageTx();
bool IDisOfInterest(const canFrame incoming);

void sendFrameToCan(canFrame frameToBeSent);
canFrame retrieveFrameFromCan();
bool loopbackCanFrame(canFrame &frameToBeSent);        // Send the can frame out on bus and loops it back on the recieving end to be read back to ground

void canbusInit();

inline uint64_t syncTimeReference;

uint16_t deltaTime();
void syncTime(canFrame timeFrame);



#endif