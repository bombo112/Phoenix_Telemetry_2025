#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    ReadyToSend = false;
    ResendLastRadioPackage = false;

    while(true)
    {
        processCanbusMessageRx(true);
        processCanbusMessageTx();

        if(ReadyToSend)                     {CanRxFifoToSend();}
        else if(!gpio_get(Pin_Radio_DIO0))  {ReceiveToCanTxFifo();}

        //if (timeToLogRocketModule())        {logger.reportRocket();}
    }
    return 0;
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
