#include "node_bakke.hpp"


void GroundLoop()
{
    watchdog_update();
    serialInit();
    ReadyToSend = true;
    ResendLastRadioPackage = false;
    LoopFromLastBroadcast = 0;

    watchdog_update();
    while(true)
    {
        processSerialMessageTx();

        if(ReadyToSend)                     {SerialRxFifoToSend();}
        else if(!gpio_get(Pin_Radio_DIO0))  {ReceiveToSerialTxFifo();}

        if(LoopFromLastBroadcast>10000)     {SendResendPackageCommand();}

        if(timeToLogGroundModule())         {logger.reportGround();}

        LoopFromLastBroadcast++;


        watchdog_update();
    }
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