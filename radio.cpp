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
        return OutgoingRadioMessage;
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage;
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
        return OutgoingRadioMessage;
    }
    else{
        RadioPackage OutgoingRadioMessage(NothingToSend);
        OutgoingRadioMessage.send();
        return OutgoingRadioMessage;
    }
}


void SendResendPackageCommand(void){
    RadioPackage OutgoingRadioMessage(ResendPackage);
    OutgoingRadioMessage.send();
    ResendLastRadioPackage = true;
}


bool ReceiveToSerialTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    if(WasThePackageSentTwice(ReceiveRadioMessage)){return 1;}
    //ReceiveRadioMessage.print(); //debug
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToSerial(CanMessages);
    }
    LastReceivedRadioPackage = ReceiveRadioMessage;
    return 1;
}


bool ReceiveToCanTxFifo(void){
    if(LoRa.parsePacket() == 0){return 0;}
    RadioPackage ReceiveRadioMessage;
    ReceiveRadioMessage.receive();
    if(WasThePackageSentTwice(ReceiveRadioMessage)){return 1;}
    ReceiveRadioMessage.print(); //debug
    int NumberOfCan = ReceiveRadioMessage.NumberOfBytes / CanFrameSize;
    for (int i = 0; i < NumberOfCan; i++){ 
        canFrame CanMessages = ReceiveRadioMessage.MessageToCan(i);
        //CanMessages.print();//debug
        sendFrameToCan(CanMessages);
    }
    if(!ResendLastRadioPackage){LastReceivedRadioPackage = ReceiveRadioMessage;}
    return 1;
}


bool WasThePackageSentTwice(RadioPackage PackageToCheck){
    if(PackageToCheck.NumberOfBytes != LastReceivedRadioPackage.NumberOfBytes){return false;}
    for (size_t i = 0; i < PackageToCheck.NumberOfBytes; i++){
        if(PackageToCheck.data[i] != LastReceivedRadioPackage.data[i]){return false;}
    }
    return true;
}
