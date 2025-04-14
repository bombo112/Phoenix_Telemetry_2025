#include "radioMessaging.hpp"


RadioPackage::RadioPackage(){NumberOfBytes = 0;}


RadioPackage::RadioPackage(uint8_t type){
//Sets the number of bytes the packet has
NumberOfBytes = CanFrameSize;

//Adds the Can Id
data[0] = InternalTelemetryMessageId & 0xFF;
data[1] = InternalTelemetryMessageId>>8;

//Adds the Can timestamp
timeStamp InternalTelemetryMessageTime = getTimeStamp();
data[CanIdSize + 0] = InternalTelemetryMessageTime.data[2];
data[CanIdSize + 1] = InternalTelemetryMessageTime.data[1];
data[CanIdSize + 2] = InternalTelemetryMessageTime.data[0];

//Adds the Can data
switch (type){
    case NothingToSend:
        data[CanIdSize + CanTimeSize  + 0] = NothingToSendData[0];
        data[CanIdSize + CanTimeSize  + 1] = NothingToSendData[1];
        data[CanIdSize + CanTimeSize  + 2] = NothingToSendData[2];
        data[CanIdSize + CanTimeSize  + 3] = NothingToSendData[3];
        data[CanIdSize + CanTimeSize  + 4] = NothingToSendData[4];
        data[CanIdSize + CanTimeSize  + 5] = NothingToSendData[5];
        data[CanIdSize + CanTimeSize  + 6] = NothingToSendData[6];
        data[CanIdSize + CanTimeSize  + 7] = NothingToSendData[7];
        break;

    case ResendPackage:
        data[CanIdSize + CanTimeSize  + 0] = ResendPackageData[0];
        data[CanIdSize + CanTimeSize  + 1] = ResendPackageData[1];
        data[CanIdSize + CanTimeSize  + 2] = ResendPackageData[2];
        data[CanIdSize + CanTimeSize  + 3] = ResendPackageData[3];
        data[CanIdSize + CanTimeSize  + 4] = ResendPackageData[4];
        data[CanIdSize + CanTimeSize  + 5] = ResendPackageData[5];
        data[CanIdSize + CanTimeSize  + 6] = ResendPackageData[6];
        data[CanIdSize + CanTimeSize  + 7] = ResendPackageData[7];
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
    IdsToPerformAction(can);
    if(NumberOfBytes>=(MaxNumberOfBytesForData - CanFrameSize))  {return 1;}
    return 0;
}


canFrame RadioPackage::MessageToCan(int CanNumber){
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
    
    switch (CanMessage.id){
    case InternalTelemetryMessageId:
        if(CanMessage.CompareCanFrameDataToArray(NothingToSendData)){break;}
        else if (CanMessage.CompareCanFrameDataToArray(ResendPackageData)){ResendLastRadioPackage = true;}
        break;

    case ResetRocketModuleId:
        if (memcmp(currentBoard.id, RocketNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0){
            if(CanMessage.CompareCanFrameDataToArray(ResetRocketModuleData)){rom_reboot(BOOT_TYPE_NORMAL, 100,0,0);}
        }
        break;

    case BootSelectRocketModuleId:
        if (memcmp(currentBoard.id, RocketNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0){
            if(CanMessage.CompareCanFrameDataToArray(BootSelectRocketModuleData)){rom_reset_usb_boot(0,0);}
        }
        break;

    case ResetGroundModuleId:
        if (memcmp(currentBoard.id, GroundNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0){
            if(CanMessage.CompareCanFrameDataToArray(ResetGroundModuleData)){rom_reboot(BOOT_TYPE_NORMAL, 100,0,0);}
        }
        break;

    case BootSelectGroundModuleId:
        if (memcmp(currentBoard.id, GroundNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0){
            if(CanMessage.CompareCanFrameDataToArray(BootSelectGroundModuleData)){rom_reset_usb_boot(0,0);}
        }
        break;

    default:
        break;
    }
}
