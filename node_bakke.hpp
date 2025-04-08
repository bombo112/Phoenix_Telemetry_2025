#include "Settings.hpp"
#include "LoRa-RP2040.h"
#include "radioMessaging.hpp"
#include "serial.hpp"
#include "radio.hpp"

#ifndef NODE_BAKKE_HPP
#define NODE_BAKKE_HPP

int GroundLoop();
inline bool timeToLogGroundModule();

#endif