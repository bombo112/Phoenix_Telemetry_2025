#include "radioMessaging.hpp"
#include <queue>
#include "serial.hpp"
#include "Settings.hpp"


#ifndef RADIO_H
#define RADIO_H


RadioPackage CanRxFifoToSend(void);
RadioPackage SerialRxFifoToSend(void);
bool ReceiveToSerialTxFifo(void);
bool ReceiveToCanTxFifo(void);


#endif
