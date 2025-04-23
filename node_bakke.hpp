#include "Settings.hpp"
#include "LoRa-RP2040.h"
#include "radioMessaging.hpp"
#include "serial.hpp"
#include "radio.hpp"
#include "hardware/watchdog.h"

#ifndef NODE_BAKKE_HPP
#define NODE_BAKKE_HPP

void GroundLoop();
inline bool timeToLogGroundModule();

#endif