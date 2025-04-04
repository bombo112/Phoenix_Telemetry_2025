#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();
    logging logger;

    bool ReadyToSend = 0;

    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();
        printf("Rakettnode\n");
        if(ReadyToSend){
            CanRxFifoToSend();
            ReadyToSend = 0;
            //sleep_us(100);
        }
        else{
            if(ReceiveToCanTxFifo()){
                ReadyToSend = 1;
            }
        }
    }
}
