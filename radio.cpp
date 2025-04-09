#include "radio.hpp"


/*
absolute_time_t picoTime1 = get_absolute_time();
absolute_time_t picoTime2 = get_absolute_time();
printf("Delta tid for CanRxFifoToMessageFifo i us: %d \n",(picoTime1-picoTime2));
*/


RadioPackage CanRxFifoToSend(void){
    sleep_us(20); //gives the receiver time to switch to RX
    RadioPackage OutgoingRadioMessage;
    if(canRxfifo.size()>MaxNumberOfCanFrame){//kansje send det sålenge det er noe i bufferen
        for (int i = 0; i < MaxNumberOfCanFrame; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromCan());
        }
        OutgoingRadioMessage.send();
    }
    else{
        canFrame NothingToSend; //lag denne global så den ikke er gjenskapt hver syklus untatt av timestamp og indikator på at forrige pakke var mista elle noe sånt++ //kan kasje gjøre dette derekte i RadioPackage construkter
        NothingToSend.id = NothingToSendId;
        NothingToSend.delta = deltaTime();
        memcpy(NothingToSend.data, NothingToSendData, sizeof(NothingToSendData));
        OutgoingRadioMessage.CanToMessage(NothingToSend);
        OutgoingRadioMessage.send();
    }
    return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
}


RadioPackage SerialRxFifoToSend(void){
    sleep_us(20); //gives the receiver time to switch to RX
    RadioPackage OutgoingRadioMessage;
    if(serialRxfifo.size()>MaxNumberOfCanFrame){
        for (int i = 0; i < MaxNumberOfCanFrame; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromSerial());
        }
        OutgoingRadioMessage.send();
    }
    else{
        canFrame NothingToSend;
        NothingToSend.id = NothingToSendId;
        NothingToSend.delta = deltaTime();
        memcpy(NothingToSend.data, NothingToSendData, sizeof(NothingToSendData));
        OutgoingRadioMessage.CanToMessage(NothingToSend);
        OutgoingRadioMessage.send();
    }
    return OutgoingRadioMessage; //husk å implementerer resend hvis pakke drop
}


bool ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    //ReceiveRadioMessage.print(); //for tesating
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        CanMessages.print();//for tesating
        sendFrameToSerial(CanMessages);//sender til fifo
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
        CanMessages.print();//for tesating
        sendFrameToCan(CanMessages);//sender til fifo
    }
    return 1;
}
