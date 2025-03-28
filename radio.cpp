#include "radio.hpp"

int TypeForId(can_frame can){
    int id = can.can_id;
    for (uint32_t i = 0; i < sizeof(Type1Ids); i++)
    {
        if (id == Type1Ids[i]) {return 1;}
    }
    for (uint32_t i = 0; i < sizeof(Type10Ids); i++)
    {
        if (id == Type10Ids[i]) {return 10;}
    }
    for (uint32_t i = 0; i < sizeof(Type20Ids); i++)
    {
        if (id == Type20Ids[i]) {return 20;}
    }
    for (uint32_t i = 0; i < sizeof(Type30Ids); i++)
    {
        if (id == Type30Ids[i]) {return 30;}
    }
    return 0;
}


void MessageToFifo(message message){
    switch(message.type){
        case 1:
            if (type1fifo.size() >= MaxBufferSize){type1fifo.pop();}
            type1fifo.push(message);
            break;
        case 10:
            if (type10fifo.size() >= MaxBufferSize){type10fifo.pop();}
            type10fifo.push(message);
            break;
        case 20:
            if (type20fifo.size() >= MaxBufferSize){type20fifo.pop();}
            type20fifo.push(message);
            break;
        case 30:
            if (type30fifo.size() >= MaxBufferSize){type30fifo.pop();}
            type30fifo.push(message);
            break;
        default:
            break;
        
    }
}

void CanToMessegeFifo(can_frame CanFrame){
    switch (TypeForId(CanFrame)){
        case 1:
            if(RadioMessageType1.CanToMessage(CanFrame))
            {
                MessageToFifo(RadioMessageType1);
                RadioMessageType1.length = 0;
            }
            break;
        case 10:
            if(RadioMessageType10.CanToMessage(CanFrame))
            {
                MessageToFifo(RadioMessageType10);
                RadioMessageType10.length = 0;
            }
            break;
        case 20:
            if(RadioMessageType20.CanToMessage(CanFrame))
            {
                MessageToFifo(RadioMessageType20);
                RadioMessageType20.length = 0;
            }
            break;
        case 30:
            if(RadioMessageType30.CanToMessage(CanFrame))
            {
                MessageToFifo(RadioMessageType30);
                RadioMessageType30.length = 0;
            }
            break;
        
        default:
            break;
    }
}

void FifoToSend(void){
    message message;
    if(!type1fifo.empty()){
        message = type1fifo.front(); 
        type1fifo.pop(); 
        message.send();
    }
    else if(!type10fifo.empty()){
        message = type1fifo.front(); 
        type1fifo.pop(); 
        message.send();
    }
    else if(!type20fifo.empty()){
        message = type1fifo.front(); 
        type1fifo.pop(); 
        message.send();
    }
    else if(!type30fifo.empty()){
        message = type1fifo.front(); 
        type1fifo.pop(); 
        message.send();
    }
    else{}//melingen som sier at det ikke er noe å sende
}
