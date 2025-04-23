#include "canbus.hpp"
#include "logging.hpp"
#include "radioMessaging.hpp"
#include "radio.hpp"
#include "hardware/watchdog.h"

#ifndef NODE_RAKETT_HPP
#define NODE_RAKETT_HPP

void RocketLoop();
inline bool timeToLogRocketModule();

#endif