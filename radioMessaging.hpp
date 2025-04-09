#include "canbus.hpp"
#include <stdio.h>
#include "LoRa-RP2040.h"
#include "Settings.hpp"
#include "logging.hpp"


#ifndef MESSAGE_H
#define MESSAGE_H


inline canFrame CanSizeMessurment;


static constexpr uint8_t CanIdSize = sizeof(CanSizeMessurment.id);
static constexpr uint8_t CanDeltaSize = sizeof(CanSizeMessurment.delta);
static constexpr uint8_t CanDataSize = sizeof(CanSizeMessurment.data);
static constexpr uint8_t CanFrameSize = CanIdSize + CanDeltaSize + CanDataSize;
static constexpr uint8_t MaxNumberOfBytesForData = 252;
static constexpr uint8_t MaxNumberOfCanFrame = MaxNumberOfBytesForData/CanFrameSize;


class RadioPackage {
    private:
        uint8_t data[MaxNumberOfBytesForData];
        int rssi;
        float snr;

    public:
        uint8_t NumberOfBytes;

        RadioPackage();
        ~RadioPackage();
        void send(void);
        void receive(void);
        void print(void);
        bool CanToMessage(canFrame can);
        canFrame MessageToCan(int CanNumber);
    };

    
#endif
