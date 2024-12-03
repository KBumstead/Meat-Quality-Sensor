
#ifndef DHT22
#define DHT22

#include <avr/io.h>
void requestData();
uint8_t readBit();
uint32_t readData();
float getTemperature(uint32_t data);

#endif
