
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <vector>
#include <cstring>



/*
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

//Definerer SPI port, frekvens og pinner.
#define RADIO_SPI_PORT spi0
#define RADIO_SPI_FREKVENS 1000000
#define RADIO_MISO 16
#define RADIO_CS   17
#define RADIO_SCK  18
#define RADIO_MOSI 19

//Funksjoner
void radio_spi_init(void);
void radio_init(void);
void radio_skriv(uint8_t adresse, uint8_t data);
uint8_t radio_les(uint8_t adresse);
void radio_les_registere(void);
void radio_send(uint8_t *data, uint8_t lengde);
void radio_mota();


void radio_spi_init(void){
    spi_init(RADIO_SPI_PORT, RADIO_SPI_FREKVENS);
    gpio_set_function(RADIO_MISO, GPIO_FUNC_SPI);
    gpio_set_function(RADIO_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(RADIO_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(RADIO_CS,   GPIO_FUNC_SIO);
    gpio_set_dir(RADIO_CS, GPIO_OUT);
    gpio_put(RADIO_CS, 1);
}

void radio_init(void){
    radio_skriv(0x01,0x08); //Setter radioen i SLEEP modus
    radio_skriv(0x01,0x88); //Setter radioen til LoRa
    radio_skriv(0x01,0x89); //Setter radioen i STDBY modus
}

void radio_skriv(uint8_t adresse, uint8_t data){
    adresse = adresse | 0x80; //Setter første bit til 1 for skriving
    gpio_put(RADIO_CS, 0); //Setter CS lav
    spi_write_blocking(RADIO_SPI_PORT, &adresse, 1);
    spi_write_blocking(RADIO_SPI_PORT, &data, 1);
    gpio_put(RADIO_CS, 1); //Setter CS høy
}

uint8_t radio_les(uint8_t adresse){
    uint8_t data;
    gpio_put(RADIO_CS, 0); //Setter CS lav
    spi_write_blocking(RADIO_SPI_PORT, &adresse, 1);
    spi_read_blocking(RADIO_SPI_PORT, 0, &data, 1);
    gpio_put(RADIO_CS, 1); //Setter CS høy
    return data;
}

void radio_les_registere(void){ //Leser av alle registerene
    uint8_t adresse;
    for (adresse = 0; adresse < 0x3c; adresse++){
            printf("Adresse: 0x%x Verdi: 0x%x\n", adresse, radio_les(adresse));
        }
}

void radio_send(uint8_t *data, uint8_t lengde){
    radio_skriv(0x01, 0x8b); //Setter radioen i TX modus
    uint8_t adresse =  0x80; //Setter første bit til 1 for skriving
    gpio_put(RADIO_CS, 0); //Setter CS lav
    spi_write_blocking(RADIO_SPI_PORT, &adresse, 1);
    spi_write_blocking(RADIO_SPI_PORT, data, lengde);
    gpio_put(RADIO_CS, 1); //Setter CS høy
}

void radio_mota(){
    uint8_t data[255];
    uint8_t nummer;
    //radio_skriv(0x01, 0x8d);
    gpio_put(RADIO_CS, 0);
    spi_write_blocking(RADIO_SPI_PORT, 0x00, 1);
    spi_read_blocking(RADIO_SPI_PORT, 0, data, 255);
    gpio_put(RADIO_CS, 1);
    for (nummer = 0; nummer < 255; nummer++){
        printf("Nummer: 0x%x Verdi: 0x%x\n", nummer, data[nummer]);
    }
}

*/

class TransmissionPacket
{
public:
    uint8_t length_of_payload;
    int32_t timestamp;
    uint8_t ID;
    uint8_t* payload;
    TransmissionPacket(size_t Length_of_payload) : length_of_payload(Length_of_payload)
    {
        payload = new uint8_t[length_of_payload];
    }
    ~TransmissionPacket()
    {
        delete[] payload;
    }
};




class sx1278
{
private:
    static constexpr uint8_t RegFifo                = 0x00; // FIFO read/write access
    static constexpr uint8_t RegOpMode              = 0x01; // Operating mode & LoRa / FSK selection
    static constexpr uint8_t RegFrfMsb              = 0x06; // RF Carrier Frequency, Most Significant Bits
    static constexpr uint8_t RegFrfMid              = 0x07; // RF Carrier Frequency, Intermediate Bits
    static constexpr uint8_t RegFrfLsb              = 0x08; // RF Carrier Frequency, Least Significant Bits
    static constexpr uint8_t RegPaConfig            = 0x09; // PA selection and Output Power control
    static constexpr uint8_t RegPaRamp              = 0x0A; // Control of PA ramp time, low phase noise PLL
    static constexpr uint8_t RegOcp                 = 0x0B; // Over Current Protection control
    static constexpr uint8_t RegLna                 = 0x0C; // LNA settings
    static constexpr uint8_t RegFifoAddrPtr         = 0x0D; // FIFO SPI pointer
    static constexpr uint8_t RegFifoTxBaseAddr      = 0x0E; // Start Tx data
    static constexpr uint8_t RegFifoRxBaseAddr      = 0x0F; // Start Rx data
    static constexpr uint8_t RegFifoRxCurrentAddr   = 0x10; // Start address of last packet received
    static constexpr uint8_t RegIrqFlagsMask        = 0x11; // Optional IRQ flag mask
    static constexpr uint8_t RegIrqFlags            = 0x12; // IRQ flags
    static constexpr uint8_t RegRxNbBytes           = 0x13; // Number of received bytes
    static constexpr uint8_t RegRxHeaderCntValueMsb = 0x14; // Number of valid headers received
    static constexpr uint8_t RegRxHeaderCntValueLsb = 0x15; // Number of valid headers received
    static constexpr uint8_t RegRxPacketCntValueMsb = 0x16; // Number of valid packets received
    static constexpr uint8_t RegRxPacketCntValueLsb = 0x17; // Number of valid packets received
    static constexpr uint8_t RegModemStat           = 0x18; // Live LoRa modem status
    static constexpr uint8_t RegPktSnrValue         = 0x19; // Espimation of last packet SNR
    static constexpr uint8_t RegPktRssiValue        = 0x1A; // RSSI of last packet
    static constexpr uint8_t RegRssiValue           = 0x1B; // Current RSSI
    static constexpr uint8_t RegHopChannel          = 0x1C; // FHSS start channe
    static constexpr uint8_t RegModemConfig_1       = 0x1D; // Modem PHY config 1
    static constexpr uint8_t RegModemConfig_2       = 0x1E; // Modem PHY config 2
    static constexpr uint8_t RegSymbTimeoutLsb      = 0x1F; // Receiver timeout value
    static constexpr uint8_t RegPreambleMsb         = 0x20; // Size of preamble
    static constexpr uint8_t RegPreambleLsb         = 0x21; // Size of preamble
    static constexpr uint8_t RegPayloadLength       = 0x22; // LoRa payload length
    static constexpr uint8_t RegMaxPayloadLength    = 0x23; // LoRa maximum payload length
    static constexpr uint8_t RegHopPeriod           = 0x24; // FHSS Hop period
    static constexpr uint8_t RegFifoRxByteAddr      = 0x25; // Address of last byte written in FIFO
    static constexpr uint8_t RegModemConfig_3       = 0x26; // Modem PHY config 3
    static constexpr uint8_t RegFeiMsb              = 0x28; // Estimated frequency error
    static constexpr uint8_t RegFeiMid              = 0x29; // Estimated frequency error
    static constexpr uint8_t RegFeiLsb              = 0x2A; // Estimated frequency error
    static constexpr uint8_t RegRssiWideband        = 0x2C; // Wideband RSSI measurement
    static constexpr uint8_t RegDetectOptimize      = 0x31; // LoRa detection Optimize for SF6
    static constexpr uint8_t RegInvertIQ            = 0x33; // Invert LoRa I and Q signals
    static constexpr uint8_t RegDetectionThreshold  = 0x37; // LoRa detection threshold for SF6
    static constexpr uint8_t RegSyncWord            = 0x39; // LoRa Sync Word
    static constexpr uint8_t RegDioMapping_1        = 0x40; // Mapping of pins DIO0 to DIO3
    static constexpr uint8_t RegDioMapping_2        = 0x41; // Mapping of pins DIO4 and DIO5, ClkOut frequency
    static constexpr uint8_t RegVersion             = 0x42; // Semtech ID relating the silicon revision
    static constexpr uint8_t RegTcxo                = 0x4B; // TCXO or XTAL input setting
    static constexpr uint8_t RegPaDac               = 0x4D; // Higher power settings of the PA
    static constexpr uint8_t RegFormerTemp          = 0x5B; // Stored temperature during the former IQ Calibration
    static constexpr uint8_t RegAgcRef              = 0x61; // Adjustment of the AGC thresholds
    static constexpr uint8_t RegAgcThresh_1         = 0x62; // Adjustment of the AGC thresholds
    static constexpr uint8_t RegAgcThresh_2         = 0x63; // Adjustment of the AGC thresholds
    static constexpr uint8_t RegAgcThresh_3         = 0x64; // Adjustment of the AGC thresholds
    static constexpr uint8_t RegPll                 = 0x70; // Control of the PLL bandwidth


    int32_t Pin_CS;
    int32_t Pin_SCK;
    int32_t Pin_MISO;
    int32_t Pin_MOSI;
    spi_inst_t *spi_port;
    int32_t SPI_Freq;

    void writeReg(uint8_t reg, uint8_t data);
    uint8_t readReg(uint8_t reg);


public:
    sx1278(int32_t CS, int32_t SCK, int32_t MISO, int32_t MOSI, spi_inst_t *PORT, int32_t SPI_FREQUENCY);
    ~sx1278();

    void sendPacket(TransmissionPacket packet);
    TransmissionPacket receivePacket(void);
};


