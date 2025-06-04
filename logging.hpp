#include "pico/stdlib.h"
#include <cstring>
#include <algorithm>
#include <queue>
#include "canbus.hpp"
#include "serial.hpp"
#include "hardware/watchdog.h"

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
        WATCHDOG_HAS_REBOOTED,
        STATUS_COUNT
    } Indicators;

    int RSSI;                         // Recieved Signal Strength Indicator
    int SNR;                          // Signal to Noies Ratio
    uint16_t bufferMeasurements[BUFFER_COUNT];
    bool statusBits[STATUS_COUNT];
    uint16_t lostPackages;
    uint16_t timeSinceBoot;
    
public:
    uint32_t uplinkMessageCount;
    uint32_t downlinkMessageCount;
    uint32_t lostMessageCount;

    inline void logSNR(int newReading)            {SNR  = std::max(newReading,  SNR);}
    inline void logRSSI(int newReading)           {RSSI = std::min(newReading, RSSI);}
    inline void iterateUplinkMessageCount(void)     {uplinkMessageCount++;}
    inline void iterateDownlinkMessageCount(void)   {downlinkMessageCount++;}
    inline void iterateLostMessageCount(void)       {lostMessageCount++;}

    void report(bool nodeIsRocket);
    void reportGround() {report(false);}
    void reportRocket() {report(true);}
    
    Logger();
    ~Logger();
};

inline Logger logger;

inline bool notFrozenIndicator() {
    static absolute_time_t lastLoggingTime = get_absolute_time();
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(lastLoggingTime, now) >= 1500000) {
        lastLoggingTime = now;
        return true;
    }
    return false;
}


#endif