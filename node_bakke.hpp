#ifndef NODE_BAKKE_HPP
#define NODE_BAKKE_HPP
#include "mcp2515.h"
#include "Settings.hpp"
#include <queue>
#include <iostream>
#include <sstream>
#include <vector>


int GroundLoop();
void PrintCan(can_frame can);
void UsbToCan();
std::vector<std::string> split(const std::string& s, char delimiter);

#endif