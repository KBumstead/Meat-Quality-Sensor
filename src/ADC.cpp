#include "ADC.h"
#include <avr/io.h>

void initADC()
{
    // below to voltage reference with external capacitor at AREF PIN
    ADMUX |= (1 << REFS0);  // This is used to control the voltage reference, setting the REFS0 as 1 enables the external power supply so it uses the 5V
    ADMUX &= ~(1 << REFS1); // Basically clears it so that the to confirm REFS0 useing the external power supply
   
    // Enable ADC and set prescaler to 128 (16 MHz/128 = 125 KHz ADC clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    DIDR0 |= (1 << ADC0D);// we disable the digital input buffer which is used to constantly monitor the voltage level so it can reduce noise, but it can interfere with the converting so we disable it
}

uint16_t readADC(uint8_t channel) {
    // Set the ADC channel dynamically
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Select ADC channel, preserve reference voltage

    // Start the conversion
    ADCSRA |= (1 << ADSC);

    // Wait for the conversion to complete
    while (!(ADCSRA & (1 << ADIF))); // Check if conversion is complete
    ADCSRA |= (1 << ADIF);           // Clear the ADC interrupt flag by writing 1

    // Return the ADC result (10-bit resolution)
    return ADC;
}