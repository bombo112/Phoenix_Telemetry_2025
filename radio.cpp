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
    message OutgoingRadioMessage;
    if(canRxfifo.size()>=MaxNumberOfCan){
        canFrame nextCanFrame; // spør jens for å fjerne denne
        for (int i = 0; i < MaxNumberOfCan; i++){
            retriveNextCanFrame(nextCanFrame);
            OutgoingRadioMessage.CanToMessage(nextCanFrame);
        }
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
}


void SerialRxFifoToSend(void){
    message OutgoingRadioMessage;
    if(serialRxfifo.size()>=MaxNumberOfCan){
        canFrame nextCanFrame; // spør jens for å fjerne denne
        for (int i = 0; i < MaxNumberOfCan; i++){
            retriveNextSerialFrame(nextCanFrame);
            OutgoingRadioMessage.CanToMessage(nextCanFrame);
        }
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
        //appendNextSerialFrame(CanMessages[i]);//sender til fifo
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
        sendCanFrame(CanMessages[i]);//sender til fifo
    }

    return 1;
}
