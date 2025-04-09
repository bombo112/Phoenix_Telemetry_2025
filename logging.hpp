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
        RUNNING,
        NODE,
        STATUS_COUNT
    } Indicators;


    float RSSI;                         // Recieved Signal Strength Indicator
    float SNR;                          // Signal to Noies Ratio
    uint16_t bufferMeasurements[BUFFER_COUNT];
    bool statusBits[STATUS_COUNT];
    //legg til mistede og sånn
    
public:
    uint32_t uplinkMessageCount;
    uint32_t downlinkMessageCount;

    inline void logSNR(float newReading)            {SNR  = std::max(newReading,  SNR);}
    inline void logRSSI(float newReading)           {RSSI = std::min(newReading, RSSI);}
    inline void iterateUplinkMessageCount(void)     {uplinkMessageCount++;}
    inline void iterateDownlinkMessageCount(void)   {downlinkMessageCount++;}

    void reportRocket();
    void reportGround();
    
    Logger();
    ~Logger();
};

inline Logger logger;



#endif