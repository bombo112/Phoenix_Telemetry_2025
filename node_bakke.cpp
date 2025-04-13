#include "node_bakke.hpp"


int GroundLoop()
{
    serialInit();
    
    bool ReadyToSend = true;
    int LoopsFromLastSend = 0;
    RadioPackage LastRadioPackage;

    while(true)
    {
        //processSerialMessageTx();
        LoopsFromLastSend++;

        if(ReadyToSend){
            if(ResendLastRadioPackage)      {LastRadioPackage.send();}
            else                            {LastRadioPackage = SerialRxFifoToSend();}
            logger.iterateUplinkMessageCount();
            ReadyToSend = false;
            ResendLastRadioPackage = false;
            LoopsFromLastSend = 0;
        }
        else if(ReceiveToSerialTxFifo()){
            logger.iterateDownlinkMessageCount();
            ReadyToSend = true;
        }

        if(LoopsFromLastSend>200)
        {
            SendResendPackageCommand();
            logger.iterateLostMessageCount();
            //logger.iterateUplinkMessageCount();
            LoopsFromLastSend = 0;
        }

        //printf("uplinkMessageCount: %d \n",logger.uplinkMessageCount);
        //printf("downlinkMessageCount: %d \n",logger.downlinkMessageCount);
        //printf("lostMessageCount: %d \n",logger.lostMessageCount);

        //if (timeToLogGroundModule())    {logger.reportGround();}
    }
    return 0;
}



inline bool timeToLogGroundModule()
{
    static absolute_time_t lastLoggingTime = get_absolute_time();
    return (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval);
}