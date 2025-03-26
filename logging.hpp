#include "pico/stdlib.h"
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <queue>
#include "mcp2515.h"
#include "canbus.hpp"

#ifndef LOGGING_HPP
#define LOGGING_HPP




class logging
{
private:
    typedef enum {
        CANRX,
        CANTX,
        SEND0,
        SEND1,
        SEND2,
        SEND3,
        USBRX,
        USBTX,
        BUFFER_COUNT
    } BufferIndex;

    typedef enum {
        RUNNING,
        NODE,
        STATUS_COUNT
    } Indicators;


    float RSSI;                         // Recieved Signal Strength Indicator
    float SNR;                          // Signal to Noies Ratio
    uint16_t buffers[BUFFER_COUNT];
    uint32_t uplinkMessageCount;
    uint32_t downlinkMessageCount;
    bool statusBits[STATUS_COUNT];
    
public:
    inline void logSNR(float newReading)           {SNR  = std::max(newReading,  SNR);}
    inline void logRSSI(float newReading)          {RSSI = std::min(newReading, RSSI);}
    inline void iterateUplinkMessageCount(void)    {uplinkMessageCount++;}
    inline void iterateDownlinkMessageCount(void)  {downlinkMessageCount++;}

    void report(canbusInterface canbus);
    void report(/*usbInterface usb*/);
    
    logging();
    ~logging();
};





#endif