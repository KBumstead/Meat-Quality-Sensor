#ifndef MQ135_H
#define MQ135_H
#include <stdint.h>

// ADC Initialization and Reading Functions
void initADC();                  // Initializes ADC for MQ-135 sensor
uint16_t readADC(uint8_t channel); // Reads ADC values for MQ-135 sensor

// MQ-135 Specific Functions
float calculatePPM(float rs_ro_ratio); // Calculates gas concentration in PPM
float getRsRoRatio(uint16_t adcValue); // Gets the Rs/R0 ratio from the ADC value
float calculateMaxDeviation(float *ppmBuffer, int bufferSize);
float calculateAveragePPM(float *ppmBuffer, int bufferSize);

#endif // MQ135_H
