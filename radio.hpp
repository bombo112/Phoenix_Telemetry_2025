#include "radioMessaging.hpp"
#include <queue>
#include "serial.hpp"


#ifndef RADIO_H
#define RADIO_H


inline std::queue<message> MessageFIFO; //gjør ett ellerannet med fifo elller drop den og bruk can fifo


void CanRxFifoToMessageFifo(void);
void SerialRxFifoToMessageFifo(void);
message MessageFifoToSend(void);
//void CanRxFifoToSend(void);
//void SerialRxFifoToSend(void);
bool ReceiveToSerialTxFifo(void);
bool ReceiveToCanTxFifo(void);



#endif
