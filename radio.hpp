#include "radioMessaging.hpp"
#include <queue>
#include "serial.hpp"
#include "Settings.hpp"


#ifndef RADIO_H
#define RADIO_H

inline RadioPackage LastReceivedRadioPackage;

RadioPackage CanRxFifoToSend(void);
RadioPackage SerialRxFifoToSend(void);
void SendResendPackageCommand(void);
bool ReceiveToSerialTxFifo(void);
bool ReceiveToCanTxFifo(void);
bool WasThePackageSentTwice(RadioPackage PackageToCheck);


#endif
