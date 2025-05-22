#include "canbus.hpp"
#include <stdio.h>
#include "LoRa-RP2040.h"
#include "Settings.hpp"
#include "logging.hpp"
#include "pico/bootrom.h"
#include "pico/bootrom_constants.h"


#ifndef RadioPackage_H
#define RADIOPACKAGE_H

inline bool ReadyToSend;
inline bool ResendLastRadioPackage;
inline int LoopFromLastBroadcast;

const uint8_t NothingToSend = 0;
const uint8_t ResendPackage = 1;

inline canFrame CanSizeMessurment;

static constexpr uint8_t CanIdSize = sizeof(CanSizeMessurment.id);
static constexpr uint8_t CanTimeSize = sizeof(CanSizeMessurment.time);
static constexpr uint8_t CanDataSize = sizeof(CanSizeMessurment.data);
static constexpr uint8_t CanFrameSize = CanIdSize + CanTimeSize + CanDataSize;
static constexpr uint8_t MaxNumberOfBytesForData = 252;
static constexpr uint8_t MaxNumberOfCanFrame = MaxNumberOfBytesForData/CanFrameSize;


class RadioPackage {
    private:
        int rssi;
        int8_t snr;

    public:
        uint8_t data[MaxNumberOfBytesForData];
        uint8_t NumberOfBytes;

        RadioPackage();
        RadioPackage(uint8_t type);
        ~RadioPackage();
        void send(void);
        void receive(void);
        void print(void);
        void CanToPackage(canFrame can);
        canFrame PackageToCan(int CanNumber);
        void IdsToPerformAction(canFrame can);
    };

    
#endif
