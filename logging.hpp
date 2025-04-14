#include "pico/stdlib.h"
#include <cstring>
#include <algorithm>
#include <queue>
#include "canbus.hpp"
#include "serial.hpp"

#ifndef LOGGING_HPP
#define LOGGING_HPP




class Logger
{
private:
    typedef enum {
        CANRX,
        CANTX,
        USBRX,
        USBTX,
        BUFFER_COUNT
    } BufferIndex;

    typedef enum {
        RUNNING_INDICATOR,
        NOT_FROZEN_INDICATOR,
        NODE_INDICATOR,
        CAN_RX_OVERFLOW_INDICATOR,
        CAN_TX_OVERFLOW_INDICATOR,
        SERIAL_RX_OVERFLOW_INDICATOR,
        SERIAL_TX_OVERFLOW_INDICATOR,
        STATUS_COUNT
    } Indicators;

    float RSSI;                         // Recieved Signal Strength Indicator
    float SNR;                          // Signal to Noies Ratio
    uint16_t bufferMeasurements[BUFFER_COUNT];
    bool statusBits[STATUS_COUNT];
    uint16_t lostPackages;
    uint16_t timeSinceBoot;
    
public:
    uint32_t uplinkMessageCount;
    uint32_t downlinkMessageCount;
    uint32_t lostMessageCount;

    inline void logSNR(float newReading)            {SNR  = std::max(newReading,  SNR);}
    inline void logRSSI(float newReading)           {RSSI = std::min(newReading, RSSI);}
    inline void iterateUplinkMessageCount(void)     {uplinkMessageCount++;}
    inline void iterateDownlinkMessageCount(void)   {downlinkMessageCount++;}
    inline void iterateLostMessageCount(void)   {lostMessageCount++;}

    void reportRocket();
    void reportGround();
    
    Logger();
    ~Logger();
};

inline Logger logger;



#endif