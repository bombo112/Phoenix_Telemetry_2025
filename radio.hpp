#include "radioMessaging.hpp"
#include <queue>

#ifndef RADIO_H
#define RADIO_H

inline std::queue<message> MessageFIFO;

void MessageToFifo(message message);
void FifoToSend(void);


#endif
