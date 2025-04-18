#include "logging.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::report(bool nodeIsRocket)
{
    bufferMeasurements[CANRX] = static_cast<uint16_t>(canRxfifo.size());
    bufferMeasurements[CANTX] = static_cast<uint16_t>(canTxfifo.size());
    bufferMeasurements[USBRX] = static_cast<uint16_t>(serialRxfifo.size());
    bufferMeasurements[USBTX] = static_cast<uint16_t>(serialTxfifo.size());
    timeSinceBoot = to_ms_since_boot(get_absolute_time());
    statusBits[CANRX] = canRxfifo.size() >= MaxBufferSize;
    statusBits[CANTX] = canTxfifo.size() >= MaxBufferSize;
    statusBits[USBRX] = serialRxfifo.size() >= MaxBufferSize;
    statusBits[USBTX] = serialTxfifo.size() >= MaxBufferSize;

    canFrame buffers;           // CanRx    CanTx   UsbRx   UsbTx
    canFrame messageCounters;   // downlink         uplink
    canFrame signalQuality;     // SNR              RSSI
    canFrame status;            // 16*bool lostpackets timesinceboot

    status.id           = 300 + (!nodeIsRocket * 20);
    signalQuality.id    = 301 + (!nodeIsRocket * 20);
    messageCounters.id  = 302 + (!nodeIsRocket * 20);
    buffers.id          = 303 + (!nodeIsRocket * 20);
    printf("buffer size: %d\n", bufferMeasurements[CANRX]);
    

    status.time           = getTimeStamp();
    signalQuality.time    = getTimeStamp();
    messageCounters.time  = getTimeStamp();
    buffers.time          = getTimeStamp();

    constexpr size_t BYTES_U16 = sizeof(uint16_t);
    constexpr size_t BYTES_U32 = sizeof(uint32_t);
    constexpr size_t BYTES_F32 = sizeof(float);

    
    status.data[0] = 0;
    status.data[1] = 0;

    for (size_t i = 0; i < STATUS_COUNT && i < 16; ++i)
    {
        if (statusBits[i])
        {
            size_t byteIndex = i / 8;
            size_t bitIndex  = i % 8;
            status.data[byteIndex] |= (uint8_t)(1 << bitIndex);
        }
    }
    memcpy(&status.data[BYTES_U16 * 2], &timeSinceBoot, BYTES_U16);
    memcpy(&status.data[BYTES_U16 * 3], &lostPackages, BYTES_U16);

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

    if (nodeIsRocket)
    {
    loopbackCanFrame(status);
    loopbackCanFrame(signalQuality);
    loopbackCanFrame(messageCounters);
    loopbackCanFrame(buffers);
    }
    else
    {
    sendFrameToSerial(status);
    sendFrameToSerial(signalQuality);
    sendFrameToSerial(messageCounters);
    sendFrameToSerial(buffers);
    }
    SNR = 0;
    RSSI = 0;
}

