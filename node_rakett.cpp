#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    bool ReadyToSend = false;

    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();
        
        if(ReadyToSend)                 {
            CanRxFifoToSend();
            ReadyToSend = false;
        }
        else if(ReceiveToCanTxFifo())   {
            ReadyToSend = true;
        }






        //if (timeToLogRocketModule())    {logger.reportRocket();}
    }
}


inline bool timeToLogRocketModule()
{
    static absolute_time_t lastLoggingTime = get_absolute_time();
    return (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval);
}
