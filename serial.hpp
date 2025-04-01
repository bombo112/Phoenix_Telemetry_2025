#include <vector>
#include <string>
#include "canbus.hpp"    // for "canFrame" data typen

#ifndef SERIAL_HPP
#define SERIAL_HPP



inline std::string inputStringSerial;
inline std::queue<canFrame> serialRxfifo;
inline std::queue<canFrame> serialTxfifo;

void serialInit();
void serialReadHandler(void *param);
bool retriveNextSerialFrame(canFrame &frameToBeRecieved);
uint32_t serialSendFrames();
void appendNextSerialFrame(canFrame &frameToBeSent);
std::vector<std::string> split(const std::string &s, const char delimiter);

#endif