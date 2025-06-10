#include "canbus.hpp"


void canFrame::print()
{
    constexpr uint64_t UsHour = 3600000000;
    constexpr uint64_t UsMinute = 60000000;
    constexpr uint64_t UsSecond  = 1000000; 

    uint64_t UsTime = parseTimeStamp(time);

    uint64_t hours   = UsTime / UsHour;
    UsTime %= UsHour;

    uint64_t minutes = UsTime / UsMinute;
    UsTime %= UsMinute;

    uint64_t seconds = UsTime / UsSecond;
    uint64_t micro   = UsTime % UsSecond;

    char buffer[32];
    snprintf(buffer, 32, "%02llu:%02llu:%02llu.%06llu", hours, minutes, seconds, micro);

    printf("%d,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%s\n",
        id,
        data[0], 
        data[1], 
        data[2], 
        data[3], 
        data[4], 
        data[5], 
        data[6], 
        data[7],
        buffer);
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

bool canFrame::CompareCanFrameDataToArray(uint8_t *arry){
    for (size_t i = 0; i < 8; i++)
    {
        if(data[i] != arry[i]){return 0;}
    }
    return 1;
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


bool processCanbusMessageRx(bool printIncoming)
{
    struct can_frame canrx;
    MCP2515::ERROR error = canbus.readMessage(&canrx);
    if (error != MCP2515::ERROR_OK)                             {return false;}
    if (canrx.can_id > 999)                                     {return false;}
    canFrame canRx(canrx);
    if (printIncoming)                                          {canRx.print();}    //debug

    if (canRx.id == utcSynchFrameId)                            {syncTime(canRx);}
    if (!IDisOfInterest(canRx))                                 {return false;}
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRx.time = getTimeStamp();
    canRxfifo.push(canRx);
    return true;
}


bool processCanbusMessageTx()
{
    const size_t maxRetries = 5; //1000
    size_t tries = 0;

    if (canTxfifo.empty())                              {return false;}

    canFrame frame = canTxfifo.front();
    //frame.print();
    can_frame frame_ = frame.convert();
    MCP2515::ERROR error = canbus.sendMessage(&frame_);
    /*
    if (frame.id == 1) {    //debug for å sjekke command kom igjennom uten feil -jens
        printf("\n");
        printf("COMMAND: ");
        frame.print();
        printf("\n");}
    */

    while (error == MCP2515::ERROR_ALLTXBUSY)
    {
        if (tries > maxRetries)                         {break;}
        error = canbus.sendMessage(&frame_);
        tries++;
        sleep_us(100);
    }
    if (error == MCP2515::ERROR_FAILTX)                 {return false;}
    canTxfifo.pop();
    return true;
}


void sendFrameToCan(canFrame frameToBeSent)
{
    if(frameToBeSent.id == InternalTelemetryMessageId){return;}
    if (canTxfifo.size() >= MaxBufferSize) 
    {
        canTxfifo.pop();
    }
    canTxfifo.push(frameToBeSent);
}


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


void loopbackCanFrame(canFrame &frameToBeSent)
{
    if (canTxfifo.size() >= MaxBufferSize)                      {canTxfifo.pop();}
    canTxfifo.push(frameToBeSent);
    if (canRxfifo.size() >= MaxBufferSize)                      {canRxfifo.pop();}
    canRxfifo.push(frameToBeSent);
}


void syncTime(canFrame gpsTimeFrame)
{   
    float utc;
    memcpy(&utc, &gpsTimeFrame.data[0], sizeof(utc));

    int32_t hhmmss = static_cast<int>(utc); 

    int32_t hours = hhmmss / 10000;
    int32_t minutes  = (hhmmss % 10000) / 100;
    int32_t seconds  = hhmmss % 100;

    float frac = utc - hhmmss;

    //parse gps time can frame to micro seconds of the day
    uint64_t gpsTime =    (uint64_t)hours   * 3600ULL * 1000000ULL
                        + (uint64_t)minutes *   60ULL * 1000000ULL
                        + (uint64_t)seconds *      1000000ULL
                        + (uint64_t)(frac * 1000000.0f);
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
