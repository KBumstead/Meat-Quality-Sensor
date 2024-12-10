#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "MQ-135.h"

// Constants for MQ-135
#define RL 4000 // Load resistor in ohms
#define RO 10000 // Calibration resistance (to be adjusted)
#define AIR_RATIOS 9.83 // Clean air Rs/R0 ratio for MQ-135
#define MEASUREMENT_INTERVAL 10 // Number of seconds for the interval
float ppmBuffer[MEASUREMENT_INTERVAL]; // Buffer to store ppm readings


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

float calculateMaxDeviation(float ppmBuffer[], int length) {
    float max = ppmBuffer[0];
    float min = ppmBuffer[0];
    for (int i = 1; i < length; i++) {
        if (ppmBuffer[i] > max) {
            max = ppmBuffer[i];
        }
        if (ppmBuffer[i] < min) {
            min = ppmBuffer[i];
        }
    }
    return max - min;
}

float calculateAveragePPM(float *ppmBuffer, int bufferSize) {
    float sum = 0;
    for (int i = 0; i < bufferSize; i++) {
        sum += ppmBuffer[i];
    }
    return sum / bufferSize;
}