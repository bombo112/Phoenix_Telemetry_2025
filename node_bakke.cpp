#include "node_bakke.hpp"


int GroundLoop()
{
    serialInit();
    
    bool ReadyToSend = 1;
    int teller = 0;

    while(true)
    {
        printf("Bakkenode\n");
        if(ReadyToSend){
            SerialRxFifoToSend();
            ReadyToSend = 0;
            //sleep_us(100);
            teller++;
            printf("Sendinger: %d\n",teller);
        }
        else{
            if(ReceiveToSerialTxFifo()){
                ReadyToSend = 1;
            }
        }
    }
    return 0;
}
