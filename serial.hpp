#include <vector>
#include <string>
#include "mcp2515.h"    // for "can_frame" data typen

#ifndef SERIAL_HPP
#define SERIAL_HPP



void serialReadCAN();
std::vector<std::string> split(const std::string &s, char delimiter);

#endif