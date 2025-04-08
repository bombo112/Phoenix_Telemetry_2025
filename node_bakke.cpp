#include "node_bakke.hpp"


int GroundLoop()
{
    //serialInit();
    
    bool ReadyToSend = 1;
    int LoopsFromLastSend = 0;
    int TellerMotatt = 0;
    int TellerMistet = 0;

    while(true)
    {
        if(ReadyToSend){
            SerialRxFifoToSend();
            ReadyToSend = 0;
            LoopsFromLastSend = 0;
        }
        else if(ReceiveToSerialTxFifo()){
            ReadyToSend = 1;
            TellerMotatt++;
        }

        if(LoopsFromLastSend>200)
        {
            ReadyToSend = 1;
            TellerMistet++;
        }

        printf("TellerMotatt: %d\n",TellerMotatt);
        printf("TellerMistet: %d\n",TellerMistet);
        printf("LoopsFromLastSend: %d\n",LoopsFromLastSend);
        LoopsFromLastSend++;


        if (timeToLogGroundModule())    {logger.reportGround();}
    }
    return 0;
}



inline bool timeToLogGroundModule()
{
    static absolute_time_t lastLoggingTime = get_absolute_time();
    return (absolute_time_diff_us(lastLoggingTime, get_absolute_time()) >= loggingInterval);
}