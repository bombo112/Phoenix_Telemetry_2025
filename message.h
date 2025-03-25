#include "mcp2515.h"

static uint8_t CanIdLength = 4;
static uint8_t CanDataLength = 8;
static uint8_t MaxMessageLength = 252;


class message {
    private:
        uint8_t type;
        uint8_t *data;
        int rssi;
        float snr;
    public:
        uint8_t length;

        // Constructor: allocates memory for 'data' and copies the input array
        message(uint8_t TYPE, uint8_t LENGTH, uint8_t *DATA) : type(TYPE), length(LENGTH){
            // Allocate memory for data
            data = new uint8_t[length];
            // Copy the array content from DATA to data
            memcpy(data, DATA, length);
        }

        message(){
            // Allocate memory for data
            data = new uint8_t[256];
        }

        message(uint8_t TYPE) : type(TYPE){
            length = 0;
            // Allocate memory for data
            data = new uint8_t[256];
        }
        
        // Destructor: free the allocated memory
        ~message() {
            delete[] data;
        }
    
        void send(void){
            LoRa.beginPacket();             
            LoRa.write(type);           
            LoRa.write(length);      
            LoRa.write(data, length);               
            LoRa.endPacket(); 
            length = 0; // for testing
        }
        
        void receive(void){
            type = LoRa.read(); 
            length = LoRa.read(); 
            
            uint8_t DATA[length];
            for(int i=0; i< length; i++) {
                DATA[i] = LoRa.read();
            }
            rssi = LoRa.packetRssi();
            snr = LoRa.packetSnr();
            memcpy(data, DATA, length);
          }
        
        void print(void){
            printf("Type: %d\n", type);
            printf("Message length: %d\n", length);
            printf("Message: ");
            for(int i=0; i< length; i++){
                printf("%d ",data[i]);
            }
            printf("\n");
            printf("RSSI: %d\n", rssi);
            printf("SNR: %d\n", snr);
        }


        bool can2message(can_frame can){
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

        can_frame message2can(void){
            int id;
            can_frame can;
            can.can_id = 399;
            memcpy(can.data, data+(length-CanDataLength), CanDataLength);
            memcpy(&can.can_id, data+length-CanDataLength-CanIdLength, CanIdLength);
            length -= (CanIdLength + CanDataLength);
            return can;
        }

        void printCan(can_frame can){
            printf("%d", can.can_id);
            for(int i=0; i< 8; i++){
                printf(":%d",can.data[i]);
            }
            printf("\n");
        }

    };
