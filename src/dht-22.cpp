#include "dht-22.h"
#include <Arduino.h>

#include <avr/io.h>


void initdht22(){
    DDRE |= (1 << DDE2);
}