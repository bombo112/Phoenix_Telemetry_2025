#include "radioMessaging.hpp"


RadioPackage::RadioPackage(){NumberOfBytes = 0;}


RadioPackage::RadioPackage(uint8_t type){
//Sets the number of bytes the packet has
NumberOfBytes = CanFrameSize;

//Adds the Can Id
data[0] = InternalTelemetryMessageId;
data[1] = InternalTelemetryMessageId>>8;

//Adds the Can timestamp
uint64_t timestamp = deltaTime();
for (size_t i = 0; i < CanDeltaSize; i++){
    data[CanIdSize + i] = timestamp>>(i*8);
}

//Adds the Can data
switch (type){
    case NothingToSend:
        data[CanIdSize + CanDeltaSize  + 0] = 1;
        data[CanIdSize + CanDeltaSize  + 1] = 1;
        data[CanIdSize + CanDeltaSize  + 2] = 1;
        data[CanIdSize + CanDeltaSize  + 3] = 1;
        data[CanIdSize + CanDeltaSize  + 4] = 1;
        data[CanIdSize + CanDeltaSize  + 5] = 1;
        data[CanIdSize + CanDeltaSize  + 6] = 1;
        data[CanIdSize + CanDeltaSize  + 7] = 1;
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
    memcpy(data +NumberOfBytes + CanIdSize, &can.delta, CanDeltaSize);
    memcpy(data +NumberOfBytes + CanIdSize + CanDeltaSize, &can.data, CanDataSize);
    NumberOfBytes += CanFrameSize;
    if(NumberOfBytes>=(MaxNumberOfBytesForData - CanFrameSize))  {return 1;}
    return 0;
}


canFrame RadioPackage::MessageToCan(int CanNumber){
    canFrame can;
    memcpy(&can.id, data + (CanNumber*CanFrameSize), CanIdSize);
    memcpy(&can.delta, data + (CanNumber*CanFrameSize) + CanIdSize, CanDeltaSize);
    memcpy(&can.data, data + (CanNumber*CanFrameSize) + CanIdSize + CanDeltaSize, CanDataSize);
    return can;
}
