#include <vector>
#include <string>
#include "canbus.hpp"    // for "canFrame" data typen

#ifndef SERIAL_HPP
#define SERIAL_HPP



inline std::string inputStringSerial;
inline std::queue<canFrame> serialRxfifo;
inline std::queue<canFrame> serialTxfifo;


void processSerialMessageRx(void *param);
void processSerialMessageTx();
std::vector<std::string> split(const std::string &s, const char delimiter);

void serialInit();
canFrame retrieveFrameFromSerial();
void sendFrameToSerial(canFrame frameToBeSent);

void CheckIfResetGroundModule(canFrame FrameToCheck);

#endif