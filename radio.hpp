#include "radioMessaging.hpp"
#include <queue>

#ifndef RADIO_H
#define RADIO_H

inline std::queue<message> type1fifo;
inline std::queue<message> type10fifo;
inline std::queue<message> type20fifo;
inline std::queue<message> type30fifo;

inline message RadioMessageType1(1);
inline message RadioMessageType10(10);
inline message RadioMessageType20(20);
inline message RadioMessageType30(30);

int TypeForId(can_frame can);
void MessageToFifo(message message);
void CanToMessegeFifo(can_frame nextCanFrame);
void FifoToSend(void);


#endif
