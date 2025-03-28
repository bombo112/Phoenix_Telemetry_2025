#include "radioMessaging.hpp"

message::message(){
    // Allocate memory for data
    data = new uint8_t[MaxMessageLength];
}

// Destructor: free the allocated memory
message::~message() {
    delete[] data;
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
    memcpy(data, DATA, length);
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


bool message::CanToMessage(can_frame can){
    memcpy(data+length, &can.can_id, sizeof(can.can_id));
    memcpy(data+length+CanIdLength, &can.data, sizeof(can.data));
    length += CanIdLength + CanDataLength;
    if(length>MaxMessageLength - CanIdLength - CanDataLength){
        return 1;
    }
    else{
        return 0;
    }
}

can_frame message::MessageToCan(void){
    int id;
    can_frame can;
    can.can_id = 399;
    memcpy(can.data, data+(length-CanDataLength), CanDataLength);
    memcpy(&can.can_id, data+length-CanDataLength-CanIdLength, CanIdLength);
    length -= (CanIdLength + CanDataLength);
    return can;
}
