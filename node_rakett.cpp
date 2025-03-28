#include "node_rakett.hpp"


int RocketLoop()
{   
    canbusInit();
    logging logger;
    message radioMelling;

    
    bool KlarForSending = 1;

    //Main loop
    while(true)
    {
        processCanbusMessageRx();
        canFrame nextCanFrame;
        if (retriveNextCanFrame(nextCanFrame))
        {
            if(radioMelling.CanToMessage(nextCanFrame))
            {
                radioMelling.send();
                radioMelling.length = 0;
            }
        }
    }
}
