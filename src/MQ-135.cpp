#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "MQ-135.h"

// Constants for MQ-135
#define RL 10000 // Load resistor in ohms
#define RO 10000 // Calibration resistance (to be adjusted)
#define AIR_RATIOS 9.83 // Clean air Rs/R0 ratio for MQ-135

// ADC Initialization
void initADC() {
    ADMUX |= (1 << REFS0);  // Voltage reference = AVCC
    ADMUX &= ~(1 << REFS1); // Clear REFS1
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, prescaler = 128
    DIDR0 |= (1 << ADC0D);  // Disable digital input on ADC0
}

// ADC Reading Function
uint16_t readADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); // Select channel
    ADCSRA |= (1 << ADSC);                    // Start conversion
    while (!(ADCSRA & (1 << ADIF)));          // Wait for conversion to complete
    ADCSRA |= (1 << ADIF);                    // Clear interrupt flag
    return ADC;
}

// Calculate Gas Concentration
float calculatePPM(float rs_ro_ratio) {
    // Based on MQ-135 datasheet graph (log-log plot for NH3)
    return pow(10, (log10(rs_ro_ratio) - 0.3) / -0.5); // Equation approximated for NH3
}

// Get Rs/R0 Ratio
float getRsRoRatio(uint16_t adcValue) {
    float voltage = (adcValue / 1023.0) * 5.0;    // Convert ADC value to voltage
    float rs = (5.0 - voltage) * RL / voltage;   // Calculate sensor resistance Rs
    return rs / RO;                              // Calculate Rs/R0 ratio
}
