#include "node_rakett.hpp"


void RocketLoop()
{   
    watchdog_update();

    canbusInit();

    ReadyToSend = false;
    ResendLastRadioPackage = false;

    watchdog_update();
    while(true)
    {
        processCanbusMessageRx(false);
        processCanbusMessageTx();

        if(ReadyToSend)                     {CanRxFifoToSend();}
        else if(!gpio_get(Pin_Radio_DIO0))  {ReceiveToCanTxFifo();}

        if (timeToLogRocketModule())        {logger.reportRocket();}
        if (LoRa.readRegister(0x42) == 0x12)    {watchdog_update();}
    }
}


inline bool timeToLogRocketModule() {
    static absolute_time_t lastLoggingTime = get_absolute_time();
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(lastLoggingTime, now) >= loggingInterval) {
        lastLoggingTime = now;
        return true;
    }
    return false;
}
