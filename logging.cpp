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

    statusBits[CAN_RX_OVERFLOW_INDICATOR]       = canRxfifo.size() >= MaxBufferSize;
    statusBits[CAN_TX_OVERFLOW_INDICATOR]       = canTxfifo.size() >= MaxBufferSize;
    statusBits[SERIAL_RX_OVERFLOW_INDICATOR]    = serialRxfifo.size() >= MaxBufferSize;
    statusBits[SERIAL_TX_OVERFLOW_INDICATOR]    = serialTxfifo.size() >= MaxBufferSize;
    statusBits[WATCHDOG_HAS_REBOOTED]           = watchdog_caused_reboot();
    statusBits[NOT_FROZEN_INDICATOR]            = notFrozenIndicator();
    statusBits[RUNNING_INDICATOR]               = true;

    canFrame buffers;           // CanRx    CanTx   UsbRx   UsbTx
    canFrame messageCounters;   // downlink         uplink
    canFrame signalQuality;     // SNR              RSSI
    canFrame status;            // 16*bool lostpackets timesinceboot

    status.id           = 300 + (!nodeIsRocket * 20);
    signalQuality.id    = 301 + (!nodeIsRocket * 20);
    messageCounters.id  = 302 + (!nodeIsRocket * 20);
    buffers.id          = 303 + (!nodeIsRocket * 20);

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

            constexpr uint64_t UsHour = 3600000000;
            constexpr uint64_t UsMinute = 60000000;
            constexpr uint64_t UsSecond  = 1000000; 
            uint64_t UsTime = parseTimeStamp(getTimeStamp());

            uint64_t hours   = UsTime / UsHour;
            UsTime %= UsHour;

            uint64_t minutes = UsTime / UsMinute;
            UsTime %= UsMinute;

            uint64_t seconds = UsTime / UsSecond;
            uint64_t micro   = UsTime % UsSecond;

            char buffer[32];
            snprintf(buffer, 32, "%02llu:%02llu:%02llu.%06llu", hours, minutes, seconds, micro);

            status.print();
            signalQuality.print();
            messageCounters.print();
            buffers.print();
            printf("CANRX: %d, CANTX: %d, Time: %s, Downlink: %d, Uplink: %d, SNR: %d, RSSI: %d", bufferMeasurements[CANRX], bufferMeasurements[CANTX], buffer, downlinkMessageCount, uplinkMessageCount, SNR, RSSI);
            printf("\n\n");
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

