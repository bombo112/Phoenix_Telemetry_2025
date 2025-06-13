#include <cstdint>
#include <cstddef>
#include "pico/unique_id.h"

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

//Radio pinner
static constexpr uint8_t Pin_Radio_CS       = 17;
static constexpr uint8_t Pin_Radio_SCK      = 18;
static constexpr uint8_t Pin_Radio_MISO     = 16;
static constexpr uint8_t Pin_Radio_MOSI     = 19;
static constexpr uint8_t Pin_Radio_DIO0     = 20;
static constexpr uint8_t Pin_Radio_DIO1     = 21;
static constexpr uint8_t Pin_Radio_DIO2     = 22;
static constexpr uint8_t Pin_Radio_DIO3     = 26;
static constexpr uint8_t Pin_Radio_DIO4     = 27;
static constexpr uint8_t Pin_Radio_RESET    = 28;

//CAN-Bus Pinner
static constexpr uint8_t Pin_Can_Cs         = 13;
static constexpr uint8_t Pin_Can_MOSI       = 15;
static constexpr uint8_t Pin_Can_MISO       = 12;
static constexpr uint8_t Pin_Can_SCK        = 14;
static constexpr uint8_t Pin_Can_RESET      = 10;
static constexpr uint8_t Pin_Can_STBY       =  4;


static constexpr uint64_t RadioFrekvens   = 421E6;
static constexpr absolute_time_t loggingInterval = 1E6 * 1;  // 1 s = 1 000 000 us

constexpr uint32_t idsToSendToGround[] = {
    2, 3, 4, 5,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    200, 201, 202, 203, 204, 206, 
    300, 301, 302, 303, 399,
    400
};
static constexpr uint32_t utcSynchFrameId = 201;

constexpr size_t MaxBufferSize = 2000;

static constexpr pico_unique_board_id_t RocketNodeId    = {.id {0xD0,0x61,0x5D,0xC9,0x28,0xCC,0x1D,0x14}}; //D0615DC928CC1D14
static constexpr pico_unique_board_id_t GroundNodeId    = {.id {0xDB,0x34,0xBE,0x26,0xFB,0xDC,0x69,0xB9}}; //DB34BE26FBDC69B9
static constexpr pico_unique_board_id_t testRocketNodeId    = {.id {0xC0,0xDA,0x3D,0x12,0xB9,0x2F,0xBD,0xDE}}; //C0DA3D12B92FBDDE
static constexpr pico_unique_board_id_t testGroundNodeId    = {.id {0x1C,0x7F,0x62,0xA4,0xAA,0x51,0xDB,0x73}}; //1C7F62A4AA51DB73

const uint16_t ResetRocketModuleId          = 304;
const uint16_t ResetGroundModuleId          = 324;
const uint16_t InternalTelemetryMessageId   = 399;

static uint8_t ResetRocketModuleKey[8]  = {0,0,0,0,0,0,0,1};
static uint8_t ResetGroundModuleKey[8]  = {0,0,0,0,0,0,0,2};
static uint8_t NothingToSendData[8]     = {1,1,1,1,1,1,1,1};
static uint8_t ResendPackageData[8]     = {2,2,2,2,2,2,2,2};

#endif