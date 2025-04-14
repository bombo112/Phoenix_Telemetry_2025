#include "canbus.hpp"



void canBusLoop()
{
    while (true)
    {
        processCanbusMessageRx();
        processCanbusMessageTx();
        tight_loop_contents();
    }
}


canFrameData canFrame::toPlainData()
{
    canFrameData dataStruct;
    dataStruct.id = id;
    dataStruct.time[0] = time.data[0];
    dataStruct.time[1] = time.data[1];
    dataStruct.time[2] = time.data[2];
    for (size_t i = 0; i < 8; i++)
    {
        dataStruct.data[i] = data[i];
    }
    return dataStruct;
}

canFrame::canFrame(const canFrameData &dataStruct) {
    id = dataStruct.id;
    time.data[0] = dataStruct.time[0];
    time.data[1] = dataStruct.time[1];
    time.data[2] = dataStruct.time[2];
    for (size_t i = 0; i < 8; i++)
    {
        data[i] = dataStruct.data[i];
    }
}

void canFrame::print()
{
    //printf("ID: %d, DT:%d, DATA: %02x %02x %02x %02x %02x %02x %02x %02x\n", id, time.toInt(), data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}


canFrame::canFrame(can_frame frameToConvert)
{
   id = frameToConvert.can_id;
   time.zero();
   for (size_t i = 0; i < 8; i++)   {data[i] = frameToConvert.data[i];}
}

canFrame::canFrame()
{
    id = 0;
    time.zero();
    for (size_t i = 0; i < 8; i++)  {data[i] = 0;}
}


can_frame canFrame::convert()
{
    can_frame frameToConvert;
    frameToConvert.can_id = id;
    for (size_t i = 0; i < 8; i++)  {frameToConvert.data[i] = data[i] ;}
    return frameToConvert;
}


void canbusInit()
{
    gpio_init(Pin_Can_Cs);                  //set CS
    gpio_set_dir(Pin_Can_Cs, GPIO_OUT);
    gpio_put(Pin_Can_Cs, 1);

    gpio_init(Pin_Can_RESET );              //set RESET
    gpio_set_dir(Pin_Can_RESET , GPIO_OUT);
    gpio_put(Pin_Can_RESET , 1);

    gpio_init(Pin_Can_STBY);                //pulldown STBY
    gpio_set_dir(Pin_Can_STBY, GPIO_IN);
    gpio_pull_down(Pin_Can_STBY);

    gpio_put(Pin_Can_RESET , 0);            //hardware reset mcp25625
    sleep_ms(100);
    gpio_put(Pin_Can_RESET , 1);
    sleep_ms(2);
    
    if (canbus.reset() != MCP2515::ERROR_OK)                                {printf("CAN reset failed!\n");         rom_reboot(BOOT_TYPE_NORMAL, 10, 0, 0);}//lag reset funksjon og vent på watchdog
    if (canbus.setBitrate(CAN_1000KBPS, MCP_20MHZ) != MCP2515::ERROR_OK)    {printf("Setting bitrate failed!\n");   rom_reboot(BOOT_TYPE_NORMAL, 10, 0, 0);}
    canbus.setNormalMode();

    queue_init(&canRxfifoMultiCore, sizeof(canFrameData), MaxBufferSize);
    queue_init(&canTxfifoMultiCore, sizeof(canFrameData), MaxBufferSize);
    printf("launch cancore\n");
    multicore_reset_core1();
    multicore_launch_core1(canBusLoop);
}


bool IDisOfInterest(const canFrame incoming)
{
    uint32_t id = incoming.id;
    for (uint32_t i = 0; i < sizeof(idsToSendToGround); i++)
    {
        if (id == idsToSendToGround[i]) {return true;}
    }
    return false;
}

/*
bool processCanbusMessageRx()
{
    struct can_frame canrx;
    MCP2515::ERROR error = canbus.readMessage(&canrx);
    if ((error != MCP2515::ERROR_OK) && (error != MCP2515::ERROR_NOMSG))    {printf("ERROR RECIEVING: %d\n", error); return false;}
    canFrame canRx(canrx);

    if (!IDisOfInterest(canRx))                                 {return false;}
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRx.time = getTimeStamp();
    canRx.print();
    canRxfifo.push(canRx);
    return true;
}
*/

bool processCanbusMessageRx()
{
    struct can_frame canrx;
    MCP2515::ERROR error = canbus.readMessage(&canrx);
    if ((error != MCP2515::ERROR_OK) && (error != MCP2515::ERROR_NOMSG))    {return false;}
    canFrame canRx(canrx);

    if (!IDisOfInterest(canRx))                         {return false;}
    if (queue_is_full(&canRxfifoMultiCore))             {queue_remove_blocking(&canRxfifoMultiCore, NULL);}
    canRx.time = getTimeStamp();


    canFrame frame;
    canFrameData frameData = canRx.toPlainData();
    queue_add_blocking(&canRxfifoMultiCore, &frameData);
    return true;
}

/*
bool processCanbusMessageTx()
{
    if (canTxfifo.empty())                              {return false;}
    canFrame canTx = canTxfifo.front();                 canTxfifo.pop();
    can_frame cantx = canTx.convert();
    MCP2515::ERROR error = canbus.sendMessage(&cantx);
    if (error != MCP2515::ERROR_OK)                     {printf("ERROR SENDING: %d\n", error); return false;}
    return true;
}
*/

bool processCanbusMessageTx()
{
    const size_t maxRetries = 1000;
    size_t tries = 0;

    canFrameData dequeuedData;
    if (!queue_try_peek(&canTxfifoMultiCore, &dequeuedData))   {return false;}

    canFrame frame(dequeuedData);
    can_frame frame_ = frame.convert();
    MCP2515::ERROR error = canbus.sendMessage(&frame_);

    while (error == MCP2515::ERROR_ALLTXBUSY)
    {
        if (tries > maxRetries)                         {break;}
        error = canbus.sendMessage(&frame_);
        tries++;
    }
    if (error == MCP2515::ERROR_FAILTX)                 {return false;}
    queue_remove_blocking(&canTxfifoMultiCore, NULL);
    return true;
}

/*
void sendFrameToCan(canFrame frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize) 
    {
        canTxfifo.pop();
    }
    canTxfifo.push(frameToBeSent);
}
*/

void sendFrameToCan(canFrame frameToBeSent)
{
    if (queue_is_full(&canTxfifoMultiCore))             {queue_remove_blocking(&canTxfifoMultiCore, NULL);}
    canFrame frame;
    canFrameData frameData = frameToBeSent.toPlainData();
    queue_add_blocking(&canTxfifoMultiCore, &frameData);
}

/*
canFrame retrieveFrameFromCan()
{
    canFrame frameToBeRecieved;
    if(!canRxfifo.empty())
    {
        frameToBeRecieved = canRxfifo.front();
        canRxfifo.pop();
        return frameToBeRecieved;
    }
    else
    {
        return frameToBeRecieved;
    }
}
*/

canFrame retrieveFrameFromCan()
{
    canFrame frameToBeRecieved;
    if (queue_is_empty(&canRxfifoMultiCore))
    {
        return frameToBeRecieved;
    }
    canFrameData dequeuedData;
    queue_remove_blocking(&canRxfifoMultiCore, &dequeuedData);
    canFrame convertedFrame(dequeuedData);

    return convertedFrame;
}

/*
void loopbackCanFrame(canFrame &frameToBeSent)
{
    printf("LOOPBACK, Tx %d Rx %d\n", canTxfifo.size(), canRxfifo.size());
    if (canTxfifo.size() >= MaxBufferSize)                      {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(frameToBeSent);
}
*/

void loopbackCanFrame(canFrame &frameToBeSent)
{
    /*
    //printf("LOOPBACK, Tx %d Rx %d\n", queue_get_level(&canTxfifoMultiCore), queue_get_level(&canRxfifoMultiCore));
    if (queue_is_full(&canTxfifoMultiCore))                 {queue_remove_blocking(&canTxfifoMultiCore, NULL);}
    queue_add_blocking(&canTxfifoMultiCore, &frameToBeSent);
    if (queue_is_full(&canRxfifoMultiCore))                 {queue_remove_blocking(&canRxfifoMultiCore, NULL);}
    queue_add_blocking(&canRxfifoMultiCore, &frameToBeSent);
    */
}



void syncTime(canFrame gpsTimeFrame)
{
    uint64_t gpsTime = 0;                                       //parse gps time can frame to micro seconds of the day
    uint64_t picoTime = to_us_since_boot(get_absolute_time());

    utcPicoDeltaTime = absolute_time_diff_us(picoTime, gpsTime);
}




timeStamp getTimeStamp()
{
    uint64_t picoTime = to_us_since_boot(get_absolute_time());
    uint64_t microseconds =  picoTime + utcPicoDeltaTime;

    uint64_t ticks = (microseconds * ticksPerDay) / usPerDay;
    timeStamp stamp(ticks);
    return stamp;
}


uint64_t parseTimeStamp(timeStamp stamp)
{
    uint64_t ticks = stamp.toInt();
    uint64_t microseconds = (ticks * usPerDay) / ticksPerDay;
    return microseconds;
}
