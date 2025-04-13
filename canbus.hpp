#include <queue>
#include <cstdio>
#include <string>
#include "mcp2515.h"
#include "Settings.hpp"
#include "pico/time.h"
#include "pico/bootrom.h"
#include "pico/bootrom_constants.h"

#ifndef CANBUS_HPP
#define CANBUS_HPP



class timeStamp
{
public:
    uint8_t data[3];

    void zero()  {data[0] = 0; data[1] = 0; data[2] = 0;}

    uint64_t toInt()
    {
        return  (static_cast<uint64_t>(data[0]) << 16) |
                (static_cast<uint64_t>(data[1]) << 8)  |
                (static_cast<uint64_t>(data[2]));
    }
    timeStamp(uint64_t utcTick)
    {
        data[0] = (utcTick>>16) & 0xFF;
        data[1] = (utcTick>>8) & 0xFF;
        data[2] = (utcTick>>0) & 0xFF;
    }
    timeStamp()     {zero();}
    ~timeStamp()    {}
};



class canFrame
{
public:
    uint16_t id;
    timeStamp time;
    uint8_t data[8];

    void print();
    canFrame(can_frame frameToConvert);
    can_frame convert();
    canFrame();
    bool CompareCanFrameDataToArray(uint8_t *arry);
};

inline std::queue<canFrame> canRxfifo;
inline std::queue<canFrame> canTxfifo;
inline MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000);

bool processCanbusMessageRx();
bool processCanbusMessageTx();
bool IDisOfInterest(const canFrame incoming);

void sendFrameToCan(canFrame frameToBeSent);
canFrame retrieveFrameFromCan();
void loopbackCanFrame(canFrame &frameToBeSent);     // Send the can frame out on bus and loops it back on the recieving end to be read back to ground

void canbusInit();





constexpr uint64_t ticksPerDay  = 1<<24;                //    16777216  ticks/day
constexpr uint64_t usPerDay     = 86400000000;          // 86400000000  microseconds/day
constexpr uint64_t usPerTick    = usPerDay/ticksPerDay; //        5149  microseconds/tick

static uint64_t utcPicoDeltaTime;



void syncTime(canFrame gpsTimeFrame);
timeStamp getTimeStamp();
absolute_time_t parseTimeStamp(timeStamp time);


#endif