#include "radio.hpp"

/*
void MessageToMessageFifo(message message){
    if (MessageFIFO.size() >= MaxBufferSize){MessageFIFO.pop();}
    MessageFIFO.push(message);
}

void CanToMessageFifo(canFrame can){
    if(radioMessage.CanToMessage(can))
    {
        MessageToMessageFifo(radioMessage);
        radioMessage.length = 0;
    }
}

void MessageFifoToSend(void){
    message OutgoingRadioMessage;
    if(!MessageFIFO.empty()){
        OutgoingRadioMessage = MessageFIFO.front(); 
        MessageFIFO.pop(); 
        OutgoingRadioMessage.send();
    }
    else{
        canFrame NothingToSend;
        NothingToSend.id = NothingToSendId;
        NothingToSend.delta = 0; //legg til 
        memcpy(NothingToSend.data, NothingToSendData, sizeof(NothingToSendData));
        OutgoingRadioMessage.CanToMessage(NothingToSend);
        OutgoingRadioMessage.send();
    }
}*/


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
        NothingToSend.delta = deltaTime(); //legg til 
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
        NothingToSend.delta = deltaTime(); //legg til 
        memcpy(NothingToSend.data, NothingToSendData, sizeof(NothingToSendData));
        OutgoingRadioMessage.CanToMessage(NothingToSend);
        OutgoingRadioMessage.send();
    }
}


bool ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    message ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.length/CanLength;
    canFrame CanMessages[NumberOfCan];
    for (int i = NumberOfCan-1; i > -1; i--){
        CanMessages[i] = ReceiveRadioMessage.MessageToCan();
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
        CanMessages[i] = ReceiveRadioMessage.MessageToCan();
    }
    for (int i = 0; i < NumberOfCan; i++){ 
        CanMessages[i].print();//for tesating
        sendFrameToCan(CanMessages[i]);//sender til fifo
    }

    return 1;
}
