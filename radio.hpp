#include "radioMessaging.hpp"
#include <queue>

#ifndef RADIO_H
#define RADIO_H

inline message radioMessage;

inline std::queue<message> MessageFIFO;

void MessageToMessageFifo(message message);
void CanToMessageFifo(canFrame can);
void MessageFifoToSend(void);
bool ReceiveToUsbTxFifo(void);
bool ReceiveToCanTxFifo(void);



#endif
