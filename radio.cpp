#include "radio.hpp"


void CanRxFifoToMessageFifo(void){
    message RadioMessage;
    if(canRxfifo.size()>MaxNumberOfCan){
        for (int i = 0; i < MaxNumberOfCan; i++){
            RadioMessage.CanToMessage(retrieveFrameFromCan());
        }
        if (MessageFIFO.size() >= MaxBufferSize){MessageFIFO.pop();}
        MessageFIFO.push(RadioMessage);
    }
}


void SerialRxFifoToMessageFifo(void){
    message RadioMessage;
    if(serialRxfifo.size()>MaxNumberOfCan){
        for (int i = 0; i < MaxNumberOfCan; i++){
            RadioMessage.CanToMessage(retrieveFrameFromSerial());
        }
        if (MessageFIFO.size() >= MaxBufferSize){MessageFIFO.pop();}
        MessageFIFO.push(RadioMessage);
    }
}


message MessageFifoToSend(void){
    sleep_us(20);   //beskriv hvorfor den er her i kommentar
    message OutgoingRadioMessage;
    if(!MessageFIFO.empty()){
        OutgoingRadioMessage = MessageFIFO.front(); 
        MessageFIFO.pop();
        OutgoingRadioMessage.send();
    }
    else{
        canFrame NothingToSend; //lag denne global så den ikke er gjenskapt hver syklus untatt av timestamp og indikator på at forrige pakke var mista elle noe sånt++
        NothingToSend.id = NothingToSendId;
        NothingToSend.time = getTimeStamp();
        memcpy(NothingToSend.data, NothingToSendData, sizeof(NothingToSendData));
        OutgoingRadioMessage.CanToMessage(NothingToSend);
        OutgoingRadioMessage.send();
    }
    CanRxFifoToMessageFifo();
    SerialRxFifoToMessageFifo();
    return OutgoingRadioMessage;    //husk å implementerer resend hvis pakke drop
}

/*
void CanRxFifoToSend(void){
    sleep_us(20);
    message OutgoingRadioMessage;
    if(canRxfifo.size()>MaxNumberOfCan){
        for (int i = 0; i < MaxNumberOfCan; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromCan());
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
}


void SerialRxFifoToSend(void){
    sleep_us(20);
    message OutgoingRadioMessage;
    if(serialRxfifo.size()>MaxNumberOfCan){
        for (int i = 0; i < MaxNumberOfCan; i++){
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
}*/


bool ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    message ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.length / CanLength;
    canFrame CanMessages[NumberOfCan];
    for (int i = NumberOfCan-1; i > -1; i--){
        CanMessages[i] = ReceiveRadioMessage.MessageToCan(); //implementer med i istedenfpor length på messageToCan()
    }
    for (int i = 0; i < NumberOfCan; i++){
        CanMessages[i].print();//for tesating
        sendFrameToSerial(CanMessages[i]);//sender til fifo
    }
    return 1;
}


bool ReceiveToCanTxFifo(void){
    message ReceiveRadioMessage;
    if(LoRa.parsePacket() == 0){return 0;}
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.length/CanLength;
    canFrame CanMessages[NumberOfCan];
    for (int i = NumberOfCan-1; i > -1; i--){
        CanMessages[i] = ReceiveRadioMessage.MessageToCan();    //implementer med i istedenfpor length på messageToCan()
    }
    for (int i = 0; i < NumberOfCan; i++){ 
        CanMessages[i].print();//for tesating
        sendFrameToCan(CanMessages[i]);//sender til fifo
    }

    return 1;
}
