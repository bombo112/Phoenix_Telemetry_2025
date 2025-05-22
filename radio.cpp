#include "radio.hpp"

void CanRxFifoToSend(void){
    if(ResendLastRadioPackage)      {LastSentRadioPackage.send();}
    else if(!canRxfifo.empty()){
        RadioPackage OutgoingRadioPackage;
        for (int i = 0; i < MaxNumberOfCanFrame && canRxfifo.size()>0; i++){
            OutgoingRadioPackage.CanToPackage(retrieveFrameFromCan());
        }
        OutgoingRadioPackage.send();
        LastSentRadioPackage = OutgoingRadioPackage;
    }
    else{
        RadioPackage OutgoingRadioPackage(NothingToSend);
        OutgoingRadioPackage.send();
        LastSentRadioPackage = OutgoingRadioPackage;
    }
}


void SerialRxFifoToSend(void){
    if(ResendLastRadioPackage)      {LastSentRadioPackage.send();}
    else if(!serialRxfifo.empty()){
        RadioPackage OutgoingRadioMessage;
        for (int i = 0; i < MaxNumberOfCanFrame && serialRxfifo.size()>0; i++){
            OutgoingRadioMessage.CanToPackage(retrieveFrameFromSerial());
        }
        OutgoingRadioMessage.send();
        LastSentRadioPackage = OutgoingRadioMessage;
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        LastSentRadioPackage = OutgoingRadioMessage;
    }
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
        canFrame CanMessages = ReceiveRadioMessage.PackageToCan(i);
        //CanMessages.print();//debug
        sendFrameToSerial(CanMessages);
    }
    if(!ResendLastRadioPackage){LastReceivedRadioPackage = ReceiveRadioMessage;}
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
        canFrame CanMessages = ReceiveRadioMessage.PackageToCan(i);
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
