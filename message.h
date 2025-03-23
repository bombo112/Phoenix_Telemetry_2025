

class message {
    private:
        uint8_t type;
        uint8_t length;
        uint8_t *data;
        int rssi;
        float snr;
    public:
        // Constructor: allocates memory for 'data' and copies the input array
        message(uint8_t TYPE, uint8_t LENGTH, uint8_t *DATA) : type(TYPE), length(LENGTH)
        {
            // Allocate memory for data
            data = new uint8_t[length];
            // Copy the array content from DATA to data
            memcpy(data, DATA, length);
        }

        message()
        {
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
    };

    /*
    uint8_t lengde = 32;
    uint8_t data[lengde];
    for(int i=0; i<lengde; i++){
      data[i] = i;
    }
    
    message melding(1, sizeof(data), data);
    message mota;
  
    while (1) {
      melding.send();
      if(LoRa.parsePacket() != 0){
        mota.receive();
        mota.print();
      }
      sleep_ms(100);
    }
    */
   
