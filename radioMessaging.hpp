#include "canbus.hpp"
#include <stdio.h>
#include "LoRa-RP2040.h"
#include "Settings.hpp"

#ifndef MESSAGE_H
#define MESSAGE_H

class message {
    private:
        uint8_t *data;
        int rssi;
        float snr;
    public:
        uint8_t length;

        message();
        ~message();
        void send(void);
        void receive(void);
        void print(void);
        bool CanToMessage(canFrame can);
        canFrame MessageToCan(void);
    };

#endif
