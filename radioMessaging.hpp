#include "canbus.hpp"
#include <stdio.h>
#include "LoRa-RP2040.h"
#include "Settings.hpp"
#include "logging.hpp"
#include "pico/bootrom.h"
#include "pico/bootrom_constants.h"


#ifndef MESSAGE_H
#define MESSAGE_H

inline bool ResendLastRadioPackage = false;
inline canFrame CanSizeMessurment;

const uint8_t NothingToSend = 0;
const uint8_t ResendPackage = 1;

static constexpr uint8_t CanIdSize = sizeof(CanSizeMessurment.id);
static constexpr uint8_t CanTimeSize = sizeof(CanSizeMessurment.time);
static constexpr uint8_t CanDataSize = sizeof(CanSizeMessurment.data);
static constexpr uint8_t CanFrameSize = CanIdSize + CanTimeSize + CanDataSize;
static constexpr uint8_t MaxNumberOfBytesForData = 252;
static constexpr uint8_t MaxNumberOfCanFrame = MaxNumberOfBytesForData/CanFrameSize;


class RadioPackage {
    private:
        int rssi;
        float snr;

    public:
        uint8_t data[MaxNumberOfBytesForData];
        uint8_t NumberOfBytes;

        RadioPackage();
        RadioPackage(uint8_t type);
        ~RadioPackage();
        void send(void);
        void receive(void);
        void print(void);
        bool CanToMessage(canFrame can);
        canFrame MessageToCan(int CanNumber);
        void IdsToPerformAction(canFrame can);
    };

    
#endif
