#include "node_bakke.hpp"


int GroundLoop()
{
    serialInit();
    
    bool ReadyToSend = true;
    int LoopsFromLastSend = 0;
    RadioPackage LastRadioPackage;

    while(true)
    {
        processSerialMessageTx();
        LoopsFromLastSend++;

        if(ReadyToSend){
            if(ResendLastRadioPackage)      {LastRadioPackage.send();}
            else                            {LastRadioPackage = SerialRxFifoToSend();}
            logger.iterateUplinkMessageCount();
            ReadyToSend = false;
            ResendLastRadioPackage = false;
            LoopsFromLastSend = 0;
        }
        else if(!gpio_get(Pin_Radio_DIO0)){
            if(ReceiveToSerialTxFifo()){
                logger.iterateDownlinkMessageCount();
                ReadyToSend = true;
            }
        }

        if(LoopsFromLastSend>10000)
        {
            SendResendPackageCommand();
            logger.iterateLostMessageCount();
            //logger.iterateUplinkMessageCount();
            LoopsFromLastSend = 0;
        }

        //printf("uplinkMessageCount: %d \n",logger.uplinkMessageCount);
        //printf("downlinkMessageCount: %d \n",logger.downlinkMessageCount);
        //printf("lostMessageCount: %d \n",logger.lostMessageCount);
        //printf("LoopsFromLastSend: %d \n",LoopsFromLastSend);

        //if (timeToLogGroundModule())    {logger.reportGround();}
    }
    return 0;
}



inline bool timeToLogGroundModule() {
    static absolute_time_t lastLoggingTime = get_absolute_time();
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(lastLoggingTime, now) >= loggingInterval) {
        lastLoggingTime = now;
        return true;
    }
    return false;
}