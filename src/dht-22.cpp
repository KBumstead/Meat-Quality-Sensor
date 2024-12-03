#include "dht-22.h"
#include "timer.h"
#include <Arduino.h>

#include <avr/io.h>

uint8_t data[5]; // Array to store 5 bytes of data

void requestData()
{

    DDRD |= (1 << DDD2);     // Set Pin 2 as output (sending start signal)
    PORTD &= ~(1 << PORTD2); // Pull the line LOW
    delay(18);               // Wait 18 ms
    PORTD |= (1 << PORTD2);  // Pull the line HIGH
    delayMicroseconds(40);   // Wait 20-40 µs for DHT to respond

    // Set DHT pin as input to read the data
    DDRD &= ~(1 << DDD2); // Set Pin 2 as input
}

uint8_t readBit()
{
    uint8_t value = 0;

    // Wait for the line to go high
    while (!(PIND & (1 << PIND2)))
    {
        // Do nothing, wait for pin to go high
    }

    // Wait for the line to stay high for a short period to read the bit
    delayMicroseconds(30); // Wait for 30 µs

    if (PIND & (1 << PIND2))
    {
        value = 1; // If the pin stays high, it's a '1'
    }

    // Wait for the line to go low after reading the bit
    while (PIND & (1 << PIND2))
    {
        // Do nothing, wait for pin to go low
    }

    return value;
}

uint32_t readData()
{
    uint32_t data = 0;

    for (int i = 0; i < 40; i++)
    {
        data <<= 1;        // Shift the data left by one bit
        data |= readBit(); // Read the next bit and store it in the data variable
    }

    return data;
}
float getTemperature(uint32_t data)
{
    uint8_t highByte = (data >> 24) & 0xFF; // Extract the high byte
    uint8_t lowByte = (data >> 16) & 0xFF;  // Extract the low byte

    // DHT11 sends the temperature in the first byte, no decimal part
    return (float)highByte;
}
