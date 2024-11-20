#include "dht-22.h"
#include "timer.h"
#include <Arduino.h>

#include <avr/io.h>

uint8_t data[5];  // Array to store 5 bytes of data

void initdht22(){
     // Set the data pin as output
    DDRE |= (1 << DDE2);
    
    // Pull LOW for 18ms to signal DHT22
    PORTE &= ~(1 << PE2);
    delayMs(18);  // Wait 18ms
    
    // Set HIGH for 30µs to finish the start signal
    PORTE |= (1 << PE2);
    delayMs(30);
    
    // Set the data pin as input to listen to the sensor response
    DDRE &= ~(1 << DDE2);
}

bool waitForSignal(uint8_t state, unsigned long timeout) {
    unsigned long start = micros();
    while ((PINE & (1 << PE2)) == state) {
        if (micros() - start > timeout) {
            return false; // Timeout if no response
        }
    }
    return true;
}

void readData() {
    for (int i = 0; i < 40; i++) {
        // Wait for the LOW signal
        waitForSignal(LOW, 50);

        // Measure the duration of the HIGH pulse
        unsigned long duration = pulseIn(PINE, HIGH);

        // Determine the bit value based on the duration
        int byteIndex = i / 8; // Which byte to store the data
        int bitIndex = 7 - (i % 8); // Bit position in the byte

        // If the pulse is longer than 40us, store 1, else store 0
        if (duration > 40) {
            data[byteIndex] |= (1 << bitIndex);
        }
    }
}

bool verifyChecksum() {
    uint8_t sum = data[0] + data[1] + data[2] + data[3];
    return (sum & 0xFF) == data[4];
}




