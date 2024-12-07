#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "MQ-135.h"

// Constants for MQ-135
#define RL 7500 // Load resistor in ohms
#define RO 10000 // Calibration resistance (to be adjusted)
#define AIR_RATIOS 9.83 // Clean air Rs/R0 ratio for MQ-135


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
