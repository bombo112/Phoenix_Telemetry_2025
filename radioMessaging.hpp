#include "canbus.hpp"
#include <stdio.h>
#include "LoRa-RP2040.h"
#include "Settings.hpp"
#include "logging.hpp"

#ifndef MESSAGE_H
#define MESSAGE_H

inline canFrame CanSizeMessurment;

static constexpr uint8_t CanIdLength = sizeof(CanSizeMessurment.id);
static constexpr uint8_t CanDeltaLength = sizeof(CanSizeMessurment.time);
static constexpr uint8_t CanDataLength = sizeof(CanSizeMessurment.data);
static constexpr uint8_t CanLength = CanIdLength + CanDeltaLength + CanDataLength;
static constexpr uint8_t MaxMessageLength = 252;
static constexpr uint8_t MaxNumberOfCan = MaxMessageLength/CanLength;

class message { //nytt nav! "radio packet"
    private:
        uint8_t data[MaxMessageLength];
        int rssi;
        float snr;
    public:
        uint8_t length; //gi nytt navn til for eksempe message byte pointer etc...

        message();
        ~message();
        void send(void);
        void receive(void);
        void print(void);
        bool CanToMessage(canFrame can);
        canFrame MessageToCan(void);
    };

#endif
