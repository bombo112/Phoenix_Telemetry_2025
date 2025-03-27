#include <cstdint>
#include <cstddef>
#include "pico/unique_id.h"

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

//Radio pinner
static constexpr uint8_t Pin_Radio_CS     = 17;
static constexpr uint8_t Pin_Radio_SCK    = 18;
static constexpr uint8_t Pin_Radio_MISO   = 16;
static constexpr uint8_t Pin_Radio_MOSI   = 19;
static constexpr uint8_t Pin_Radio_DIO0   = 20;
static constexpr uint8_t Pin_Radio_DIO1   = 21;
static constexpr uint8_t Pin_Radio_DIO2   = 22;
static constexpr uint8_t Pin_Radio_DIO3   = 26;
static constexpr uint8_t Pin_Radio_DIO4   = 27;
static constexpr uint8_t Pin_Radio_RESET  = 28;

//CAN-Bus Pinner
static constexpr uint8_t Pin_Can_Cs       = 13;
static constexpr uint8_t Pin_Can_MOSI     = 15;
static constexpr uint8_t Pin_Can_MISO     = 12;
static constexpr uint8_t Pin_Can_SCK      = 14;
static constexpr uint8_t Pin_Can_RESET    = 10;


static constexpr uint64_t RadioFrekvens   = 433E6;

constexpr uint32_t idsToSendToGround[] = {200, 201, 202, 203, 204, 300, 301, 302, 303};

constexpr size_t MaxBufferSize = 2000;

static constexpr pico_unique_board_id_t RocketNodeId = {.id {0x06,0xd7,0xea,0x02,0xbe,0xcb,0x5b,0x99}}; //06d7ea02becb5b99
static constexpr pico_unique_board_id_t GroundNodeId = {.id {0xc5,0xa8,0xe8,0x64,0xe9,0x5c,0xa6,0x46}}; //c5a8e864e95ca646




#endif