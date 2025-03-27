#include "logging.hpp"

logging::logging(/* args */)
{

}

logging::~logging()
{

}

void logging::report()
{
    can_frame buffersPartOne;   // send0    send1   send2   send3
    can_frame buffersPartTwo;   // CanRx    CanTx   UsbRx   UsbTx
    can_frame messageCounters;  // downlink         uplink
    can_frame signalQuality;    // SNR              RSSI
    can_frame status;

    status.can_id = 200;
    signalQuality.can_id = 201;
    messageCounters.can_id = 202;
    buffersPartOne.can_id = 203;
    buffersPartTwo.can_id = 204;

    constexpr size_t BYTES_U16 = sizeof(uint16_t);
    constexpr size_t BYTES_U32 = sizeof(uint32_t);
    constexpr size_t BYTES_F32 = sizeof(float);
    
    // Buffers
    memcpy(&buffersPartOne.data[BYTES_U16 * 0], &buffers[SEND0], BYTES_U16);
    memcpy(&buffersPartOne.data[BYTES_U16 * 1], &buffers[SEND1], BYTES_U16);
    memcpy(&buffersPartOne.data[BYTES_U16 * 2], &buffers[SEND2], BYTES_U16);
    memcpy(&buffersPartOne.data[BYTES_U16 * 3], &buffers[SEND3], BYTES_U16);
    
    memcpy(&buffersPartTwo.data[BYTES_U16 * 0], &buffers[CANRX], BYTES_U16);
    memcpy(&buffersPartTwo.data[BYTES_U16 * 1], &buffers[CANTX], BYTES_U16);
    memcpy(&buffersPartTwo.data[BYTES_U16 * 2], &buffers[USBRX], BYTES_U16);
    memcpy(&buffersPartTwo.data[BYTES_U16 * 3], &buffers[USBTX], BYTES_U16);
    
    // Message counters
    memcpy(&messageCounters.data[0], &downlinkMessageCount, BYTES_U32);
    memcpy(&messageCounters.data[4], &uplinkMessageCount, BYTES_U32);
    
    // Signal quality
    memcpy(&signalQuality.data[0], &SNR,  BYTES_F32);
    memcpy(&signalQuality.data[4], &RSSI, BYTES_F32);

    loopbackCanFrame(status);
    loopbackCanFrame(signalQuality);
    loopbackCanFrame(messageCounters);
    loopbackCanFrame(buffersPartOne);
    loopbackCanFrame(buffersPartTwo);



    SNR = 0;
    RSSI = 0;

}
