#include "mcp2515.h"
#include "node_bakke.hpp"
#include "LoRa-RP2040.h"
#include "message.h"

int GroundLoop()
{
    message mota;
    void printCan(can_frame can);
    char carakterer[250];
    int carakter = 0;
    
    while (0)
    {
        if(LoRa.parsePacket() != 0){
            mota.receive();
            int NumberOfCan = mota.length/(CanIdLength + CanDataLength);
            can_frame CanMessages[NumberOfCan];
            for (int i = NumberOfCan-1; i > -1; i--){
                CanMessages[i] = mota.message2can();
            }
            for (int i = 0; i < NumberOfCan; i++){
                printCan(CanMessages[i]);
            }   
          }
    }

    while (true) {
        // Poll for a character with a timeout (10ms)
        char c = getchar_timeout_us(10000);  // 10ms timeout
        if (c != PICO_ERROR_TIMEOUT) {
            if(c = '\n'){
                carakterer[carakter] = c;
                for(int i=0; i< carakter; i++){
                    printf("%c",carakterer[i]);
                }
                carakter = 0; 
            }
            else{
                carakterer[carakter] = c;
                carakter++;
            }
        }
        // Here you can add other non-blocking tasks if needed

    }
    

    return 0;
}

void printCan(can_frame can){
    printf("%d", can.can_id);
    for(int i=0; i< 8; i++){
        printf(":%d",can.data[i]);
    }
    printf("\n");
}

//can_frame usb2can(){}
