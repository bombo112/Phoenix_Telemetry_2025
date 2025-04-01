#include "node_bakke.hpp"



int GroundLoop()
{
    serialInit();
    
    bool ReadyToSend = 1;

    while(1)
    {

        if(ReadyToSend){
            MessageFifoToSend();
            ReadyToSend = 0;
        }
        else{
            ReceiveToUsbTxFifo();
            ReadyToSend = 1;
        }
    }
    return 0;
}
