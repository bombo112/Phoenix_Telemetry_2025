#include "radioMessaging.hpp"
#include <queue>
#include "serial.hpp"


#ifndef RADIO_H
#define RADIO_H

//inline message radioMessage;

inline std::queue<message> MessageFIFO;

//void MessageToMessageFifo(message message);
//void CanToMessageFifo(canFrame can);
//void MessageFifoToSend(void);
void CanRxFifoToSend(void);
void SerialRxFifoToSend(void);
bool ReceiveToSerialTxFifo(void);
bool ReceiveToCanTxFifo(void);



#endif
