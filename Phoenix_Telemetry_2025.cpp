
#include "Settings.hpp"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"
#include "LoRa-RP2040.h"
#include "node_bakke.hpp"
#include "node_rakett.hpp"
#include "radioMessaging.hpp"
#include "pico/unique_id.h"
#include "logging.hpp"
#include "canbus.hpp"

void initLoRa()
{
  LoRa.setPins(Pin_Radio_CS, Pin_Radio_RESET, Pin_Radio_DIO0);

  if  (!LoRa.begin(RadioFrekvens))  {while (true) {printf("LoRa init failed. Check your connections.\n");}}
  else                              {printf("LoRa init succeeded.\n");}
}





int main()
{
  
  stdio_init_all();
  initLoRa();

  
  
  pico_unique_board_id_t currentBoard;
  pico_get_unique_board_id(&currentBoard);
  int error = 0;
  if (memcmp(currentBoard.id, RocketNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0) {error = RocketLoop();}
  if (memcmp(currentBoard.id, GroundNodeId.id, PICO_UNIQUE_BOARD_ID_SIZE_BYTES) == 0) {error = GroundLoop();}



  //=======ERROR=HANDDELING=======//


  while (true)//bytte ut med try catch
  {
    switch (error)
    {
    case 0:
      printf("Current board ID does not match any of the known nodes, current ID: ");
      for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; i++)
      {
        printf("%02x", currentBoard.id[i]);
      }
      printf("\n");
      break;
    
    default:
      printf("ERRORCODE: %D", error);
      break;
    }
  }

  return 0;
}
