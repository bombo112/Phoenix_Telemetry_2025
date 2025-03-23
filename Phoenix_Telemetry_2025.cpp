
#include "Settings.hpp"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"
#include "LoRa-RP2040.h"
#include "node_bakke.hpp"
#include "node_rakett.hpp"
#include "message.h"



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
  
  int error = 0;
  if (NodeIsRocket == true) {error = RocketLoop();}
  else                      {error = GroundLoop();}

  while (true){printf("ERRORCODE: %D", error);}
  return 0;
}
