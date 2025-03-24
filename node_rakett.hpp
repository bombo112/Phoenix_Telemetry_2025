#include "mcp2515.h"
#include "Settings.hpp"
#include <queue>
#include <stdio.h>
#include <array>

#ifndef NODE_RAKETT_HPP
#define NODE_RAKETT_HPP

int RocketLoop();

bool IDisOfInterest(const can_frame incoming);

void processCanbusMessage(MCP2515& canbus, std::queue<can_frame>& canRxfifo);

#endif