#include "radio.hpp"

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
    message RadioMessage;
    if(!MessageFIFO.empty()){
        RadioMessage = MessageFIFO.front(); 
        MessageFIFO.pop(); 
        RadioMessage.send();
    }
    else{
        canFrame NothingToSend;
        NothingToSend.id = NothingToSendId;
        NothingToSend.delta = 0; //legg til 
        memcpy(NothingToSend.data, &NothingToSendId, sizeof(NothingToSendId));
        RadioMessage.CanToMessage(NothingToSend);
        RadioMessage.send();
    }
}

void ReceiveToUsbTxFifo(void){
    if(LoRa.parsePacket() == 0){return;}
    message ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.length/(CanIdLength + CanDeltaLength + CanDataLength);
    canFrame CanMessages[NumberOfCan];
    for (int i = NumberOfCan-1; i > -1; i--){
        CanMessages[i] = ReceiveRadioMessage.MessageToCan();
    }
    for (int i = 0; i < NumberOfCan; i++){
        CanMessages[i].print();//for tesating
        //legg inn Usb fifo
    }
}


void ReceiveToCanTxFifo(void){
    message ReceiveRadioMessage;
    if(LoRa.parsePacket() == 0){return;}
    ReceiveRadioMessage.receive();
    int NumberOfCan = ReceiveRadioMessage.length/(CanIdLength + CanDeltaLength + CanDataLength);
    canFrame CanMessages[NumberOfCan];
    for (int i = NumberOfCan-1; i > -1; i--){
        CanMessages[i] = ReceiveRadioMessage.MessageToCan();
    }
    for (int i = 0; i < NumberOfCan; i++){ 
        CanMessages[i].print();//for tesating
        sendCanFrame(CanMessages[i]);//sender til fifo
    }
}
