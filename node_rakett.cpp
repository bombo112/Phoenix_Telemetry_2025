#include "node_rakett.hpp"


int RocketLoop()
{   
    logging logger;
    int KlarForSending = true;

    //Main loop
    while(true)
    {
        processCanbusMessageRx();
        can_frame nextCanFrame;
        if (retriveNextCanFrame(nextCanFrame))
        {
            printf("Canbuss motat\n");
            CanToMessegeFifo(nextCanFrame);
        }
        if(KlarForSending){
            CanToMessegeFifo(nextCanFrame);
            FifoToSend();
        }
    }
}
