#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"

#include "mcp25625.hpp"
#include "sx1278.hpp"

#include "LoRa-RP2040.h"

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) ((void)0);
#endif





static constexpr bool isMaster = false;      //designator for om koden fungerer som ledsager i kommunikasjonen


static constexpr uint8_t Pin_Radio_CS    = 17;
static constexpr uint8_t Pin_Radio_SCK   = 18;
static constexpr uint8_t Pin_Radio_MISO  = 16;
static constexpr uint8_t Pin_Radio_MOSI  = 19;
static constexpr uint8_t Pin_Radio_DIO0  = 20;
static constexpr uint8_t Pin_Radio_DIO1  = 21;
static constexpr uint8_t Pin_Radio_DIO2  = 22;
static constexpr uint8_t Pin_Radio_DIO3  = 26;
static constexpr uint8_t Pin_Radio_DIO4  = 27;
static constexpr uint8_t Pin_Radio_RESET = 28;

void masterLoop();
void slaveLoop();

/*

int main()
{

    if (watchdog_caused_reboot())
    {
        printf("WARNING: WATCHDOG FORCED REBOOT!");
        //logge til blackbox?
    }


    //init alle system funksjoner herfra
    stdio_init_all();

    watchdog_enable(2000, false);


    //system entry
    if (isMaster)   {masterLoop();}
    else            {slaveLoop(); }
}

/*=================================================================================*/
/*=============================== MASTER LOOP =====================================*/
/*=================================================================================*/
void masterLoop()
{
    DEBUG_PRINT("Entering Master loop");

    while (true)
    {
        TransmissionPacket testpacket1(sizeof(int32_t));
        testpacket1.ID = 1;
        testpacket1.timestamp = 2024;
        testpacket1.payload[0] = 42;
        sleep_ms(500);
    }
    
}

/*================================================================================*/
/*=============================== SLAVE LOOP =====================================*/
/*================================================================================*/
void slaveLoop()
{
    DEBUG_PRINT("Entering Slave loop");

    while (true)
    {
        TransmissionPacket testpacket2 = TransmissionPacket(1);
        printf("Incoming packet, payload: %d\n", testpacket2.payload[0]);
        sleep_ms(500);
    }
    
}





using std::string;

const int csPin = Pin_Radio_CS;          // LoRa radio chip select
const int resetPin = Pin_Radio_RESET;       // LoRa radio reset
const int irqPin = Pin_Radio_DIO0;         // change for your board; must be a hardware interrupt pin

string outgoing;              // outgoing message

uint8_t msgCount = 0;            // count of outgoing messages

uint8_t localAddress = 0xFF;     // address of this device
uint8_t destination = 0xBB;      // destination to send to

long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void sendMessage(string outgoing) {
  int n = outgoing.length();
  char send[n+1];
  strcpy(send,outgoing.c_str());
  printf("Sending: %s\n",send);
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(sizeof(send)+1);        // add payload length
  LoRa.print(send);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
  // read packet header uint8_ts:
  int recipient = LoRa.read();          // recipient address
  uint8_t sender = LoRa.read();            // sender address
  uint8_t incomingMsgId = LoRa.read();     // incoming msg ID
  uint8_t incomingLength = LoRa.read();    // incoming msg length

  string incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != sizeof(incoming)) {   // check length for error
    printf("error: message length does not match length\n");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    printf("This message is not for me.\n");
    return;                             // skip rest of function
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

int main(){

  stdio_init_all();

  printf("\nLoRa Duplex\n");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    printf("LoRa init failed. Check your connections.\n");
    while (true);                       // if failed, do nothing
  }

  printf("LoRa init succeeded.\n");
  
  while (1) {
      if (to_ms_since_boot(get_absolute_time()) - lastSendTime > interval) {
      char message[] = "HeLoRa World! Slave";   // send a message
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