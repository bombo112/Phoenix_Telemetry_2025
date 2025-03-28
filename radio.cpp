#include "radio.hpp"

void MessageToFifo(message message){
    if (MessageFIFO.size() >= MaxBufferSize){MessageFIFO.pop();}
    MessageFIFO.push(message);
}

void FifoToSend(void){
    message RadioMessage;
    if(!MessageFIFO.empty()){
        RadioMessage = MessageFIFO.front(); 
        MessageFIFO.pop(); 
        RadioMessage.send();
    }
    else{}//melingen som sier at det ikke er noe å sende
}
