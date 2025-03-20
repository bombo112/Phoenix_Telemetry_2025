#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"
#include "LoRa-RP2040.h"
#include "mcp2515.h"
#include "node_bakke.hpp"
#include "node_rakett.hpp"


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

MCP2515 canbus(spi1, Pin_Can_Cs, Pin_Can_MOSI, Pin_Can_MISO, Pin_Can_SCK, 1000000); //CAN Bus interface



std::string outgoing;             // outgoing message
uint8_t msgCount = 0;             // count of outgoing messages
uint8_t localAddress = 0xAA;      // address of this device
uint8_t destination = 0xBB;       // destination to send to
long lastSendTime = 0;            // last send time
int interval = 2000;              // interval between sends

void sendMessage(std::string outgoing)
{
  int32_t n = outgoing.length();
  char send[n];
  strcpy(send,outgoing.c_str());
  printf("Sending: %s\n",send);
  LoRa.beginPacket();             // start packet
  LoRa.write(destination);        // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(sizeof(send));       // add payload length
  LoRa.print(send);               // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}

void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // if there's no packet, return
  // read packet header uint8_ts:
  int32_t recipient = LoRa.read();      // recipient address
  uint8_t sender = LoRa.read();         // sender address
  uint8_t incomingMsgId = LoRa.read();  // incoming msg ID
  uint8_t incomingLength = LoRa.read(); // incoming msg length

  char incoming[incomingLength];

  for(int i=0; i< incomingLength; i++) {
    incoming[i] = LoRa.read();
  }

  if (incomingLength != sizeof(incoming)) {   // check length for error
    printf("error: message length does not match length\n med en lengde på %d\n",sizeof(incoming));
    return;
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    printf("This message is not for me.\n");
    return;
  }

  // if message is for this device, or broadcast, print details:
  printf("Received from: 0x%x\n",sender);
  printf("Sent to: 0x%x\n", recipient);
  printf("Message ID: %d\n", incomingMsgId);
  printf("Message length: %d\n", incomingLength);
  printf("Message: %s\n", incoming);
  printf("RSSI: %d\n", LoRa.packetRssi());
  printf("Snr: %d\n", LoRa.packetSnr());
}


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
  
  while (1) {
      if (to_ms_since_boot(get_absolute_time()) - lastSendTime > interval) {
      char message[] = "HeLoRa World! Bakke Node";   // send a message
      sendMessage(message);
      printf("Sending %s\n", message);
      lastSendTime = to_ms_since_boot(get_absolute_time());            // timestamp the message
      interval = (rand()%2000) + 1000;    // 2-3 seconds
    }
    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
  }
  return 0;
}
