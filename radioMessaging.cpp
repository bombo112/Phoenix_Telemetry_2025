#include "radioMessaging.hpp"


RadioPackage::RadioPackage(){NumberOfBytes = 0;}


RadioPackage::RadioPackage(uint8_t type){
//Sets the number of bytes the packet has
NumberOfBytes = CanFrameSize;

//Adds the Can Id
data[0] = InternalTelemetryMessageId;
data[1] = InternalTelemetryMessageId>>8;

//Adds the Can timestamp
timeStamp InternalTelemetryMessageTime = getTimeStamp();
data[CanIdSize + 0] = InternalTelemetryMessageTime.data[2];
data[CanIdSize + 1] = InternalTelemetryMessageTime.data[1];
data[CanIdSize + 2] = InternalTelemetryMessageTime.data[0];

//Adds the Can data
switch (type){
    case NothingToSend:
        data[CanIdSize + CanTimeSize  + 0] = 1;
        data[CanIdSize + CanTimeSize  + 1] = 1;
        data[CanIdSize + CanTimeSize  + 2] = 1;
        data[CanIdSize + CanTimeSize  + 3] = 1;
        data[CanIdSize + CanTimeSize  + 4] = 1;
        data[CanIdSize + CanTimeSize  + 5] = 1;
        data[CanIdSize + CanTimeSize  + 6] = 1;
        data[CanIdSize + CanTimeSize  + 7] = 1;
        break;

    default:
        NumberOfBytes = 0;
        break;
    }
}


RadioPackage::~RadioPackage(){}


void RadioPackage::send(void){
    LoRa.beginPacket();
    LoRa.write(NumberOfBytes);
    LoRa.write(data, NumberOfBytes);
    LoRa.endPacket();
}


void RadioPackage::receive(void){
    NumberOfBytes = LoRa.read();
    for(int i=0; i< NumberOfBytes; i++)    {data[i] = LoRa.read();}
    rssi = LoRa.packetRssi();
    snr = LoRa.packetSnr();

    logger.logRSSI(rssi);                  //logge funksjon -jens
    logger.logSNR(snr);                    //logge funksjon -jens
}


void RadioPackage::print(void){          //debug print
    printf("Message length: %d\n", NumberOfBytes);
    printf("Message: ");
    for(int i=0; i< NumberOfBytes; i++){
        printf("%d ",data[i]);
    }
    printf("\n");
    printf("RSSI: %d\n", rssi);
    printf("SNR: %d\n", snr);
}


bool RadioPackage::CanToMessage(canFrame can){
    memcpy(data +NumberOfBytes, &can.id, CanIdSize);
    memcpy(data +NumberOfBytes + CanIdSize, &can.time, CanTimeSize);
    memcpy(data +NumberOfBytes + CanIdSize + CanTimeSize, &can.data, CanDataSize);
    NumberOfBytes += CanFrameSize;
    if(NumberOfBytes>=(MaxNumberOfBytesForData - CanFrameSize))  {return 1;}
    return 0;
}


canFrame RadioPackage::MessageToCan(int CanNumber){
    canFrame can;
    memcpy(&can.id, data + (CanNumber*CanFrameSize), CanIdSize);
    memcpy(&can.time, data + (CanNumber*CanFrameSize) + CanIdSize, CanTimeSize);
    memcpy(&can.data, data + (CanNumber*CanFrameSize) + CanIdSize + CanTimeSize, CanDataSize);
    return can;
}
