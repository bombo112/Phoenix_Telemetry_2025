#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();
    logging logger;

    bool ReadyToSend = 0;

    //Main loop
    while(true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();
       printf("Rakettnode\n");
        if(ReadyToSend){
            CanRxFifoToSend();
            ReadyToSend = 0;
        }
        else{
            if(ReceiveToCanTxFifo()){
                ReadyToSend = 1;
            }
        }
    }
}
