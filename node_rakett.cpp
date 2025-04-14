#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();

    bool ReadyToSend = false;
    RadioPackage LastRadioPackage;

    while(true)
    {
        absolute_time_t picoTime1 = get_absolute_time();
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

        //if (timeToLogRocketModule())    {logger.reportRocket();}
        absolute_time_t picoTime2 = get_absolute_time();
        int picoTime1us = to_us_since_boot(picoTime1);
        int picoTime2us = to_us_since_boot(picoTime2);
        printf("Delta tid for CanRxFifoToMessageFifo i us: %d \n",(picoTime2us-picoTime1us));
    }
}


inline bool timeToLogRocketModule()
{
    static absolute_time_t lastLoggingTime = get_absolute_time();
    return (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval);
}
