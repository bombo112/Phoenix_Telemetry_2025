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
