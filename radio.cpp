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
