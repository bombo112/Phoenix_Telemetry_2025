#include "radioMessaging.hpp"
#include <queue>
#include "serial.hpp"
#include "Settings.hpp"


#ifndef RADIO_H
#define RADIO_H

inline RadioPackage LastSentRadioPackage;
inline RadioPackage LastReceivedRadioPackage;

void CanRxFifoToSend(void);
void SerialRxFifoToSend(void);
void SendResendPackageCommand(void);
void ReceiveToSerialTxFifo(void);
void ReceiveToCanTxFifo(void);
bool WasThePackageSentTwice(RadioPackage PackageToCheck);


#endif
