#include "logging.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::reportRocket()
{
    bufferMeasurements[CANRX] = canRxfifo.size();
    bufferMeasurements[CANTX] = canTxfifo.size();
    bufferMeasurements[USBRX] = serialRxfifo.size();
    bufferMeasurements[USBTX] = serialTxfifo.size();

    canFrame buffers;           // CanRx    CanTx   UsbRx   UsbTx
    canFrame messageCounters;   // downlink         uplink
    canFrame signalQuality;     // SNR              RSSI
    canFrame status;            // 64 bool

    status.id           = 300;
    signalQuality.id    = 301;
    messageCounters.id  = 302;
    buffers.id          = 303;

    constexpr size_t BYTES_U16 = sizeof(uint16_t);
    constexpr size_t BYTES_U32 = sizeof(uint32_t);
    constexpr size_t BYTES_F32 = sizeof(float);
    
    memcpy(&buffers.data[BYTES_U16 * 0], &bufferMeasurements[CANRX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 1], &bufferMeasurements[CANTX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 2], &bufferMeasurements[USBRX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 3], &bufferMeasurements[USBTX], BYTES_U16);
    
    // Message counters
    memcpy(&messageCounters.data[0], &downlinkMessageCount, BYTES_U32);
    memcpy(&messageCounters.data[4], &uplinkMessageCount, BYTES_U32);
    
    // Signal quality
    memcpy(&signalQuality.data[0], &SNR,  BYTES_F32);
    memcpy(&signalQuality.data[4], &RSSI, BYTES_F32);

    loopbackCanFrame(status);
    loopbackCanFrame(signalQuality);
    loopbackCanFrame(messageCounters);
    loopbackCanFrame(buffers);
    loopbackCanFrame(buffers);



    SNR = 0;
    RSSI = 0;

}


void Logger::reportGround()
{
    bufferMeasurements[CANRX] = canRxfifo.size();
    bufferMeasurements[CANTX] = canTxfifo.size();
    bufferMeasurements[USBRX] = serialRxfifo.size();
    bufferMeasurements[USBTX] = serialTxfifo.size();

    canFrame buffers;           // CanRx    CanTx   UsbRx   UsbTx
    canFrame messageCounters;   // downlink         uplink
    canFrame signalQuality;     // SNR              RSSI
    canFrame status;            // 64 bool

    status.id           = 300+20;
    signalQuality.id    = 301+20;
    messageCounters.id  = 302+20;
    buffers.id          = 303+20;

    constexpr size_t BYTES_U16 = sizeof(uint16_t);
    constexpr size_t BYTES_U32 = sizeof(uint32_t);
    constexpr size_t BYTES_F32 = sizeof(float);
    
    memcpy(&buffers.data[BYTES_U16 * 0], &bufferMeasurements[CANRX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 1], &bufferMeasurements[CANTX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 2], &bufferMeasurements[USBRX], BYTES_U16);
    memcpy(&buffers.data[BYTES_U16 * 3], &bufferMeasurements[USBTX], BYTES_U16);
    
    // Message counters
    memcpy(&messageCounters.data[0], &downlinkMessageCount, BYTES_U32);
    memcpy(&messageCounters.data[4], &uplinkMessageCount, BYTES_U32);
    
    // Signal quality
    memcpy(&signalQuality.data[0], &SNR,  BYTES_F32);
    memcpy(&signalQuality.data[4], &RSSI, BYTES_F32);

    loopbackCanFrame(status);
    loopbackCanFrame(signalQuality);
    loopbackCanFrame(messageCounters);
    loopbackCanFrame(buffers);
    loopbackCanFrame(buffers);



    SNR = 0;
    RSSI = 0;
}
