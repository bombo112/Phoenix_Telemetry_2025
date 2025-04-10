#include "radio.hpp"


/*
absolute_time_t picoTime1 = get_absolute_time();
absolute_time_t picoTime2 = get_absolute_time();
printf("Delta tid for CanRxFifoToMessageFifo i us: %d \n",(picoTime1-picoTime2));
*/


RadioPackage CanRxFifoToSend(void){
    sleep_us(20); //gives the receiver time to switch to RX
    if(!canRxfifo.empty()){
        RadioPackage OutgoingRadioMessage;
        for (int i = 0; i < MaxNumberOfCanFrame && canRxfifo.size()>0; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromCan());
        }
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
    }
}


RadioPackage SerialRxFifoToSend(void){
    sleep_us(20); //gives the receiver time to switch to RX
    if(!canRxfifo.empty()){
        RadioPackage OutgoingRadioMessage;
        for (int i = 0; i < MaxNumberOfCanFrame && serialRxfifo.size()>0; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromSerial());
        }
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
    }
}


bool ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToSerial(CanMessages);
    }
    return 1;
}


bool ReceiveToCanTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToCan(CanMessages);
    }
    return 1;
}
