#include "mcp2515.h"
#include <stdio.h>
#include "LoRa-RP2040.h"

#ifndef MESSAGE_H
#define MESSAGE_H


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

        message(uint8_t TYPE, uint8_t LENGTH, uint8_t *DATA);
        message();
        message(uint8_t TYPE);
        ~message();
        void send(void);
        void receive(void);
        void print(void);
        bool CanToMessage(can_frame can);
        can_frame MessageToCan(void);
    };

#endif
