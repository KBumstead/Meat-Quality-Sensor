#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "MQ-135.h"
#include "ADC.h"
int main()
{
  uint16_t adcValue;
  float rs_ro_ratio;
  float ppm;

  // Initialize ADC
  initADC();

  while (1)
  {
    // Read ADC value
    adcValue = readADC(0); // Assuming MQ-135 is connected to ADC0

    // Calculate Rs/R0 ratio
    rs_ro_ratio = getRsRoRatio(adcValue);

    // Calculate PPM
    ppm = calculatePPM(rs_ro_ratio);

    // Debugging or display: Send ppm to UART or display
    _delay_ms(1000); // Delay for stability
  }

  return 0;
}