#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"

#include "mcp25625.hpp"
#include "sx1278.hpp"

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) ((void)0);
#endif

static constexpr bool isMaster = false;      //designator for om koden fungerer som ledsager i kommunikasjonen


static constexpr uint8_t Pin_Radio_CS   = 17;
static constexpr uint8_t Pin_Radio_SCK  = 18;
static constexpr uint8_t Pin_Radio_MISO = 16;
static constexpr uint8_t Pin_Radio_MOSI = 19;
sx1278 Radio(Pin_Radio_CS, Pin_Radio_SCK, Pin_Radio_MISO, Pin_Radio_MOSI, spi0, 1000000);

void masterLoop();
void slaveLoop();



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
        Radio.sendPacket(testpacket1);
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
        TransmissionPacket testpacket2 = Radio.receivePacket();
        printf("Incoming packet, payload: %d\n", testpacket2.payload[0]);
        sleep_ms(500);
    }
    
}