#include "sx1278.hpp"


sx1278::sx1278(int32_t CS, int32_t SCK, int32_t MISO, int32_t MOSI, spi_inst_t *PORT, int32_t SPI_FREQUENCY=1000000)
    : Pin_CS(CS), Pin_SCK(SCK), Pin_MISO(MISO), Pin_MOSI(MOSI), spi_port(PORT), SPI_Freq(SPI_FREQUENCY)
{
    spi_init(spi_port, SPI_Freq);
    gpio_set_function(Pin_MISO, GPIO_FUNC_SPI);
    gpio_set_function(Pin_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(Pin_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(Pin_CS,   GPIO_FUNC_SIO);
    gpio_set_dir(Pin_CS, GPIO_OUT);
    gpio_put(Pin_CS, 1);

    writeReg(RegOpMode, 0x08);  //Setter radioen i SLEEP modus
    writeReg(RegOpMode, 0x88);  //Setter radioen til LoRa
    writeReg(RegOpMode, 0x89);  //Setter radioen i STDBY modus
}

sx1278::~sx1278()
{
    gpio_set_function(Pin_MISO, GPIO_FUNC_NULL);
    gpio_set_function(Pin_MOSI, GPIO_FUNC_NULL);
    gpio_set_function(Pin_SCK,  GPIO_FUNC_NULL);
    gpio_set_function(Pin_CS,   GPIO_FUNC_NULL);
}

void sx1278::writeReg(uint8_t reg, uint8_t data)
{
    reg |= 0x80;
    gpio_put(Pin_CS, 0);
    spi_write_blocking(spi_port, &reg, 1);
    spi_write_blocking(spi_port, &data, 1);
    gpio_put(Pin_CS, 1);
}

uint8_t sx1278::readReg(uint8_t reg)
{
    uint8_t data;
    gpio_put(Pin_CS, 0);
    spi_write_blocking(spi_port, &reg, 1);
    spi_read_blocking(spi_port, 0, &data, 1);
    gpio_put(Pin_CS, 1);
    return data;
}

void sx1278::sendPacket(TransmissionPacket packet)
{
    constexpr uint8_t headerSize = 1 + sizeof(packet.timestamp) + 1; // = 6 bytes
    uint32_t bufferSize = headerSize + packet.length_of_payload;
    uint8_t sendingBuffer[bufferSize];

    sendingBuffer[0] = packet.length_of_payload;
    memcpy(&sendingBuffer[1], &packet.timestamp, sizeof(packet.timestamp));
    sendingBuffer[1 + sizeof(packet.timestamp)] = packet.ID;

    memcpy(&sendingBuffer[headerSize], packet.payload, packet.length_of_payload);

    writeReg(RegOpMode, 0x8B);
    uint8_t fifoWriteCmd = RegFifo | 0x80; // set MSB for write
    gpio_put(Pin_CS, 0);
    spi_write_blocking(spi_port, &fifoWriteCmd, 1);
    spi_write_blocking(spi_port, sendingBuffer, bufferSize);
    gpio_put(Pin_CS, 1);
    writeReg(RegOpMode, 0x89);
}

TransmissionPacket sx1278::receivePacket(void) {
    uint8_t totalLength = readReg(RegRxNbBytes);

    // Read the entire FIFO content into a temporary buffer.
    uint8_t buffer[totalLength];
    uint8_t fifoRxCurrentAddr = readReg(RegFifoRxCurrentAddr);
    writeReg(RegFifoAddrPtr, fifoRxCurrentAddr);
    gpio_put(Pin_CS, 0);
    spi_write_blocking(spi_port, &RegFifo, 1);
    spi_read_blocking(spi_port, 0, buffer, totalLength);
    gpio_put(Pin_CS, 1);

    // Parse header:
    uint8_t payloadLength = buffer[0];
    int32_t timestamp;
    memcpy(&timestamp, &buffer[1], sizeof(timestamp));
    uint8_t ID = buffer[1 + sizeof(timestamp)];

    // Create a packet with the payload length.
    TransmissionPacket packet(payloadLength);
    packet.timestamp = timestamp;
    packet.ID = ID;
    memcpy(packet.payload, &buffer[1 + sizeof(timestamp) + 1], payloadLength);

    return packet;
}

