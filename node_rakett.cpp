#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    bool ReadyToSend = false;
    RadioPackage LastRadioPackage;

    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();

        if(ReadyToSend){
            if(ResendLastRadioPackage)      {LastRadioPackage.send();}
            else                            {LastRadioPackage = CanRxFifoToSend();}
            logger.iterateDownlinkMessageCount();
            ReadyToSend = false;
            ResendLastRadioPackage = false;
        }
        else if(ReceiveToCanTxFifo()){
            logger.iterateUplinkMessageCount();
            ReadyToSend = true;
        }

        //printf("uplinkMessageCount: %d \n",logger.uplinkMessageCount);
        //printf("downlinkMessageCount: %d \n",logger.downlinkMessageCount);
        if (timeToLogRocketModule())    {logger.reportRocket();}
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
