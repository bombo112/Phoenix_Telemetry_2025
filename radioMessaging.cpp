#include "radioMessaging.hpp"

message::message(){
    // Allocate memory for data
    //data = new uint8_t[MaxMessageLength];
    length = 0;
}

// Destructor: free the allocated memory
message::~message() {
    //delete[] data;
}


void message::send(void){
    LoRa.beginPacket();
    LoRa.write(length);
    LoRa.write(data, length);
    LoRa.endPacket();
}


void message::receive(void){
    length = LoRa.read();
    uint8_t DATA[length];
    for(int i=0; i< length; i++) {
        DATA[i] = LoRa.read();
    }
    rssi = LoRa.packetRssi();
    snr = LoRa.packetSnr();

    logger.logRSSI(rssi);                  //logge funksjon -jens
    logger.logSNR(snr);                    //logge funksjon -jens

    memcpy(data, DATA, length);
    //printf("rssi: %d\n",rssi);
    //printf("snr: %d\n",snr);
}


void message::print(void){
    printf("Message length: %d\n", length);
    printf("Message: ");
    for(int i=0; i< length; i++){
        printf("%d ",data[i]);
    }
    printf("\n");
    printf("RSSI: %d\n", rssi);
    printf("SNR: %d\n", snr);
}


bool message::CanToMessage(canFrame can){
    memcpy(data+length, &can.id, sizeof(can.id));
    memcpy(data+length+CanIdLength, &can.delta, sizeof(can.delta));
    memcpy(data+length+CanDeltaLength+CanIdLength, &can.data, sizeof(can.data));
    length += CanLength;
    if(length>=(MaxMessageLength - CanLength)){
        return 1;
    }
    else{
        return 0;
    }
}


canFrame message::MessageToCan(void){
    canFrame can;
    memcpy(can.data, data+length-CanDataLength, CanDataLength);
    memcpy(&can.delta, data+length-CanDataLength-CanDeltaLength, CanDeltaLength);
    memcpy(&can.id, data+length-CanDataLength-CanDeltaLength-CanIdLength, CanIdLength);
    length -= CanLength;
    return can;
}
