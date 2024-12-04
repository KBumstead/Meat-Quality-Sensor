#include <DHT.h>
#include <string.h>
#include <Arduino.h>
#include "MQ-135.h"
#include "ADC.h"
#include "Timer.h"
#include "UART.h"

// Pin configuration
#define DHTPIN 2      // Digital pin where the DHT sensor is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)

#define MAXTIME 85 // Timeout for data reading

// Create DHT object
// DHT dht(DHTPIN, DHTTYPE);

int main()
{
  // Serial.begin(9600);
  initADC();
  initTimer0();
  initUART();    // Initialize UART
  delayMs(1000); // Delay to allow JDY-31 to initialize
  // // Arduino-specific initialization
  // init();             // Initialize Arduino core
  // dht.begin();        // Initialize the DHT sensor

  // while (1) // Infinite loop
  // {
  //   // Read temperature from the DHT sensor
  //   float temperature = dht.readTemperature();

  //   if (isnan(temperature))
  //   {
  //     Serial.println("Failed to read from DHT sensor!");
  //   }
  //   else
  //   {
  //     Serial.print("Temperature: ");
  //     Serial.print(temperature);
  //     Serial.println(" °C");
  //   }

  //   delay(2000); // Wait for 2 seconds between readings
  // }

  uint16_t adcValue;
  float rs_ro_ratio, ppm;

  // Main loop
  check_connection_state();
  while (1)
  {
    // Read the ADC Value
    adcValue = readADC(0);
    // Calculate Rs/R0 ratio and gas concentration
    rs_ro_ratio = getRsRoRatio(adcValue);
    ppm = calculatePPM(rs_ro_ratio);
    Serial.println(ppm);
    delayMs(1000);
    
    
  }
  return 0; // This will never be reached
}
