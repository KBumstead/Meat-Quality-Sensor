#include <DHT.h>
#include <string.h>
#include <Arduino.h>
#include <avr/io.h>
#include "MQ-135.h"
#include "ADC.h"
#include "Timer.h"
#include "UART.h"
#include "lcd2.h"

// Pin configuration
#define DHTPIN 2      // Digital pin where the DHT sensor is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)

void update_lcd();
// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

int main()
{
  // Serial.begin(9600);

  // init_i2c();
  // LCD_Init();
  // LCD_moveCursor(0, 0);
  // LCD_WriteString("test");
  initADC();
  initTimer0();
  initTimer1();

  // initLCD
  init_i2c();
  LCD_Init();

  initUART();    // Initialize UART
  delayMs(1000); // Delay to allow JDY-31 to initialize
  dht.begin();   // Initialize the DHT sensor

  uint16_t adcValue;
  float rs_ro_ratio, ppm;
  sei();

  LCD_moveCursor(0, 0);
  LCD_WriteString("Press the button to activate");

  // Main loop
  // check_connection_state();
  while (1)
  {
    // Read Temperature if needed
    float temperature = dht.readTemperature();
    // Read Humidity
    float humidity = dht.readHumidity();
    // Read the ADC Value
    adcValue = readADC(0);
    // Calculate Rs/R0 ratio and gas concentration
    rs_ro_ratio = getRsRoRatio(adcValue);
    ppm = calculatePPM(rs_ro_ratio);
    uart_transmit_string("ppm: ");
    Serial.println(ppm);
    if (humidity > 50)
    {

      if (ppm <= 0.6)
      {
        Serial.println("Fresh");
        LCD_moveCursor(0, 0);
        LCD_WriteString("T: ");
        LCD_moveCursor(1, 0);
        LCD_WriteString("FRESH");
        // LCD_WriteString("good chicken");
      }
      else if (ppm > 0.6 && ppm < 1)
      {
        Serial.println("Medium Fresh");
        LCD_moveCursor(0, 0);
        LCD_WriteString("T: ");
        LCD_moveCursor(1, 0);
        LCD_WriteString("Medium Fresh");
        // LCD_WriteString("i dont think this is okay TT");
      }
      else if (ppm >= 1)
      {
        Serial.println("Rotten");
        LCD_moveCursor(0, 0);
        LCD_WriteString("T: ");
        LCD_moveCursor(1, 0);
        LCD_WriteString("Rotten");
        // LCD_WriteString("i dont think this is okay TT");
      }
    }

    Serial.print("Temperature: ");
    Serial.println(temperature);
    delayMs(2000); // Add delay to avoid frequent readings
  }
  return 0; // This will never be reached
}
