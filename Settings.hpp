#include <cstdint>

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
static constexpr bool NodeIsRocket = 0; // 1=Rakett, 0=bakkestasjon

#endif