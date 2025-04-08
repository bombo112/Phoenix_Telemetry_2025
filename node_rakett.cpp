#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    bool ReadyToSend = 0;

    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();

        if(ReadyToSend)                 {CanRxFifoToSend(); ReadyToSend = 0;}
        else if(ReceiveToCanTxFifo())   {ReadyToSend = 1;}

        if (timeToLogRocketModule())    {logger.reportRocket();}
    }
}


inline bool timeToLogRocketModule()
{
    static absolute_time_t lastLoggingTime = get_absolute_time();
    return (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval);
}
