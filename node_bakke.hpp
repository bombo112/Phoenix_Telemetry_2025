#ifndef NODE_BAKKE_HPP
#define NODE_BAKKE_HPP
#include "Settings.hpp"
#include "LoRa-RP2040.h"
#include "radioMessaging.hpp"


int GroundLoop();
void printCan(can_frame message);

#endif