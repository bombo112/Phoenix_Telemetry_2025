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
        canFrame nextCanFrame;
        if (retriveNextCanFrame(nextCanFrame))
        {
            CanToMessageFifo(nextCanFrame);
        }
        if(ReadyToSend){
            MessageFifoToSend();
            ReadyToSend = 0;
        }
        else{
            if(ReceiveToCanTxFifo()){
                ReadyToSend = 1;
            }
        }
    }
}
