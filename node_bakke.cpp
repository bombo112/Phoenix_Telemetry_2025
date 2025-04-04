#include "node_bakke.hpp"



int GroundLoop()
{
    serialInit();
    
    bool ReadyToSend = 1;

    while(1)
    {
        printf("Bakkenode\n");
        if(ReadyToSend){
            SerialRxFifoToSend();
            ReadyToSend = 0;
        }
        else{
            if(ReceiveToSerialTxFifo()){
                ReadyToSend = 1;
            }
        }
    }
    return 0;
}
