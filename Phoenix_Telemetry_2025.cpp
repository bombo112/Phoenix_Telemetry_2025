
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp2515.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"
#include "LoRa-RP2040.h"
#include "mcp2515.h"
#include "node_bakke.hpp"
#include "node_rakett.hpp"
#include "message.h"
#include "datamanagement.hpp"


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

MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000); //CAN Bus interface


void initLoRa()
{
  LoRa.setPins(Pin_Radio_CS, Pin_Radio_RESET, Pin_Radio_DIO0);

  if  (!LoRa.begin(RadioFrekvens))  {printf("LoRa init failed. Check your connections.\n");while (true);}
  else                              {printf("LoRa init succeeded.\n");}
}

int main()
{
  stdio_init_all();
  initLoRa();
  
  /*
  if (NodeIsRocket == true) {RocketLoop(can);}
  else                      {GroundLoop();}
  //unreachable
  */

  return 0;
}
