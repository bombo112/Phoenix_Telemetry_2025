#include "radioMessaging.hpp"


RadioPackage::RadioPackage(){NumberOfBytes = 0;}


RadioPackage::RadioPackage(uint8_t type){
//Sets the number of bytes the packet has
NumberOfBytes = CanFrameSize;

//Adds the Can Id
memcpy(data, &InternalTelemetryMessageId, CanIdSize);

//Adds the Can timestamp
timeStamp InternalTelemetryMessageTime = getTimeStamp();
memcpy(data + CanIdSize, &InternalTelemetryMessageTime.data, CanTimeSize);

//Adds the Can data
switch (type){
    case NothingToSend:
        memcpy(data + CanIdSize + CanTimeSize, &NothingToSendData, CanDataSize);
        break;

    case ResendPackage:
        memcpy(data + CanIdSize + CanTimeSize, &ResendPackageData, CanDataSize);
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
    logger.iterateUplinkMessageCount();
    LoopFromLastBroadcast = 0;
    ReadyToSend = false;
    ResendLastRadioPackage = false;
}


void RadioPackage::receive(void){
    NumberOfBytes = LoRa.read();
    for(int i=0; i< NumberOfBytes; i++)    {data[i] = LoRa.read();}
    rssi = LoRa.packetRssi();
    snr = LoRa.packetSnr();

    logger.logRSSI(rssi);                  //logge funksjon -jens
    logger.logSNR(snr);                    //logge funksjon -jens
    logger.iterateDownlinkMessageCount();
    ReadyToSend = true;
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


bool RadioPackage::CanToPackage(canFrame can){
    memcpy(data +NumberOfBytes, &can.id, CanIdSize);
    memcpy(data +NumberOfBytes + CanIdSize, &can.time, CanTimeSize);
    memcpy(data +NumberOfBytes + CanIdSize + CanTimeSize, &can.data, CanDataSize);
    NumberOfBytes += CanFrameSize;
    IdsToPerformAction(can);
    if(NumberOfBytes>=(MaxNumberOfBytesForData - CanFrameSize))  {return 1;}
    return 0;
}


canFrame RadioPackage::PackageToCan(int CanNumber){
    canFrame can;
    memcpy(&can.id, data + (CanNumber*CanFrameSize), CanIdSize);
    memcpy(&can.time, data + (CanNumber*CanFrameSize) + CanIdSize, CanTimeSize);
    memcpy(&can.data, data + (CanNumber*CanFrameSize) + CanIdSize + CanTimeSize, CanDataSize);
    IdsToPerformAction(can);
    return can;
}


void RadioPackage::IdsToPerformAction(canFrame CanMessage){
    pico_unique_board_id_t currentBoard;
    pico_get_unique_board_id(&currentBoard);
    switch (CanMessage.id)
    {
    case InternalTelemetryMessageId:
        if(CanMessage.CompareCanFrameDataToArray(NothingToSendData))        {break;}
        else if (CanMessage.CompareCanFrameDataToArray(ResendPackageData))  {ResendLastRadioPackage = true;}
        break;

    case ResetRocketModuleId:
        if ((memcmp(currentBoard.id, RocketNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0) && CanMessage.CompareCanFrameDataToArray(ResetRocketModuleKey))
        {
            rom_reboot(BOOT_TYPE_NORMAL, 100,0,0);
        }
        break;

    case ResetGroundModuleId:
        if ((memcmp(currentBoard.id, GroundNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0) && CanMessage.CompareCanFrameDataToArray(ResetGroundModuleKey))
        {
            rom_reboot(BOOT_TYPE_NORMAL, 100,0,0);
        }
        break;

    default:
        break;
    }
}
