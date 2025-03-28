#include "node_rakett.hpp"


int RocketLoop()
{   
    logging logger;
    message radioMelling;

    //Main loop
    while(true)
    {
        processCanbusMessageRx();
        can_frame nextCanFrame;
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
