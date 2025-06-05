#include "radio.hpp"


/*
absolute_time_t picoTime1 = get_absolute_time();
absolute_time_t picoTime2 = get_absolute_time();
int picoTime1us = to_us_since_boot(picoTime1);
int picoTime2us = to_us_since_boot(picoTime2);
printf("Delta tid for CanRxFifoToMessageFifo i us: %d \n",(picoTime2us-picoTime1us));
*/


void CanRxFifoToSend(void){
    if(ResendLastRadioPackage)      {LastSentRadioPackage.send();}
    else if(!canRxfifo.empty()){
        RadioPackage OutgoingRadioMessage;
        for (int i = 0; i < MaxNumberOfCanFrame && canRxfifo.size()>0; i++){
            OutgoingRadioMessage.CanToMessage(retrieveFrameFromCan());
        }
        OutgoingRadioMessage.send();
        LastSentRadioPackage = OutgoingRadioMessage;
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        LastSentRadioPackage = OutgoingRadioMessage;
    }
    ResendLastRadioPackage = false;
}


void SerialRxFifoToSend(void){
    if(ResendLastRadioPackage)      {LastSentRadioPackage.send();}
    else if(!serialRxfifo.empty()){
        bool SavePackage = 0;
        RadioPackage OutgoingRadioMessage;
        for (int i = 0; i < MaxNumberOfCanFrame && serialRxfifo.size()>0; i++){
            SavePackage = OutgoingRadioMessage.CanToMessage(retrieveFrameFromSerial());
        }
        OutgoingRadioMessage.send();
        if(SavePackage){LastSentRadioPackage = OutgoingRadioMessage;}
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        LastSentRadioPackage = OutgoingRadioMessage;
    }
    ResendLastRadioPackage = false;
}


void SendResendPackageCommand(void){
    RadioPackage OutgoingRadioMessage(ResendPackage);
    OutgoingRadioMessage.send();
    ResendLastRadioPackage = true;
    logger.iterateLostMessageCount();
}


void ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    if(WasThePackageSentTwice(ReceiveRadioMessage)){return;}
    //ReceiveRadioMessage.print(); //debug
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToSerial(CanMessages);
    }
    LastReceivedRadioPackage = ReceiveRadioMessage;
    return;
}


void ReceiveToCanTxFifo(void){
    if(LoRa.parsePacket() == 0){return;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    if(WasThePackageSentTwice(ReceiveRadioMessage)){return;}
    //ReceiveRadioMessage.print(); //debug
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToCan(CanMessages);
    }
    if(!ResendLastRadioPackage){LastReceivedRadioPackage = ReceiveRadioMessage;}
    return;
}


bool WasThePackageSentTwice(RadioPackage PackageToCheck){
    if(PackageToCheck.NumberOfBytes != LastReceivedRadioPackage.NumberOfBytes){return false;}
    for (size_t i = 0; i < PackageToCheck.NumberOfBytes; i++){
        if(PackageToCheck.data[i] != LastReceivedRadioPackage.data[i]){return false;}
    }
    return true;
}
