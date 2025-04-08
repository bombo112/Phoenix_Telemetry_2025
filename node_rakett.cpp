#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    bool ReadyToSend = 0;

    absolute_time_t lastLoggingTime = get_absolute_time();

    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();
        if(ReadyToSend){
            CanRxFifoToSend();
            ReadyToSend = 0;
        }
        else if(ReceiveToCanTxFifo())
        {
            ReadyToSend = 1;
        }

        if (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval)
        {
            logger.reportRocket();
        }
    }
}
