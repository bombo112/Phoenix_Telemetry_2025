#include "message.hpp"
#include <queue>

#ifndef RADIO_H
#define RADIO_H

std::queue<message> type1fifo;
std::queue<message> type10fifo;
std::queue<message> type20fifo;
std::queue<message> type30fifo;

void MessageToFifo(message message);
void FifoToSend(void);
int TypeForId(can_frame can);

#endif
