#include "node_rakett.hpp"


int RocketLoop()
{   
    logging logger;
    message radioMelling;

    
    bool KlarForSending = 1;

    //Main loop
    while(true)
    {
        processCanbusMessageRx();
        can_frame nextCanFrame;
        if (retriveNextCanFrame(nextCanFrame))
        {
            CanToMessegeFifo(nextCanFrame);
        }
        if(KlarForSending){
            FifoToSend();
        }
    }
}
