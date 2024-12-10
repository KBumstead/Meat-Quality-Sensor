#include <DHT.h>
#include <string.h>
#include <Arduino.h>
#include <avr/io.h>
#include "MQ-135.h"
#include "ADC.h"
#include "Timer.h"
#include "UART.h"
#include "lcd2.h"
#include "switch.h"

// Pin configuration
#define DHTPIN 2      // Digital pin where the DHT sensor is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define MEASUREMENT_INTERVAL 10 // Number of seconds for the interval

typedef enum
{
  RELEASED,
  DB_RELEASED,
  PRESSED,
  DB_PRESSED,
} machineState;

volatile machineState MachineState = RELEASED;

typedef enum
{
  FRESH,
  ROTTING,
  ROTTEN,
  idle
} meatState;

volatile meatState MeatState = idle;

// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

int main()
{

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

  // init the switch
  initSwitchPD2();

  initUART();    // Initialize UART

  uint16_t adcValue;
  float rs_ro_ratio, ppm;
  float ppmBuffer[MEASUREMENT_INTERVAL];
  sei();

  LCD_moveCursor(0, 0);
  LCD_WriteString("Press the button to activate");

  // Main loop
  // check_connection_state();
  float temperature;
  float humidity;
  char tempStr[10];
  char humidityStr[10];
  char ppmStr[10];
  char maxDeviationStr[10];
  char avgPPMStr[10];
  while (1)
  {

    if (MachineState == RELEASED)
    {
      if (MeatState == idle)
      {
        LCD_Clear();
        LCD_moveCursor(0, 0);
        LCD_WriteString("Push the button to read");
      }
      else if (MeatState == FRESH)
      {
        LCD_moveCursor(1, 0);
        LCD_WriteString("FRESH MEAT");
      }
      else if (MeatState == ROTTING)
      {
        LCD_moveCursor(1, 0);
        LCD_WriteString("ROTTING MEAT");
      }
      else if (MeatState == ROTTEN)
      {
        LCD_moveCursor(1, 0);
        LCD_WriteString("ROTTEN MEAT");
      }
    }
    else if (MachineState == PRESSED)
    {
      dht.begin(); // Initialize the DHT sensor
      // dht sensor activate
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
      dtostrf(temperature, 4, 1, tempStr);  // Convert temperature to string (4 characters wide, 1 decimal place)
      dtostrf(humidity, 4, 1, humidityStr); // Convert humidity to string (4 characters wide, 1 decimal place)
      uart_transmit_string("Temperature: ");
      uart_transmit_string(tempStr);
      uart_transmit_string("\n");
      uart_transmit_string("Humidity:");
      uart_transmit_string(humidityStr);
      uart_transmit_string("\n");
      LCD_Clear();
      if (humidity > 50)
      {
        for (int i = 0; i < MEASUREMENT_INTERVAL; i++)
        {
          adcValue = readADC(0);
          rs_ro_ratio = getRsRoRatio(adcValue);
          ppmBuffer[i] = calculatePPM(rs_ro_ratio);

          dtostrf(ppmBuffer[i], 4, 1, ppmStr);
          uart_transmit_string("PPM:");
          uart_transmit_string(ppmStr);
          uart_transmit_string("\n");

          // Display countdown on the LCD
          int remainingTime = MEASUREMENT_INTERVAL - i; // Remaining seconds
          LCD_Clear();
          LCD_moveCursor(0, 0);
          LCD_WriteString("Time remaining: ");
          char countdownStr[10];
          itoa(remainingTime, countdownStr, 10); // Convert remaining time to string
          LCD_moveCursor(1, 0);
          LCD_WriteString(countdownStr);
          LCD_WriteString(" s");
          delayMs(1000); // Delay 1 second between measurements
        }
        float maxDeviation = calculateMaxDeviation(ppmBuffer, MEASUREMENT_INTERVAL);
        float avgPPM = calculateAveragePPM(ppmBuffer, MEASUREMENT_INTERVAL);
        dtostrf(maxDeviation, 4, 1, maxDeviationStr);
        uart_transmit_string("PPM Max Deviation :");
        uart_transmit_string(maxDeviationStr);
        uart_transmit_string("\n");
        dtostrf(avgPPM, 4, 1, avgPPMStr);
        uart_transmit_string("Average PPM:");
        uart_transmit_string(avgPPMStr);
        uart_transmit_string("\n");

        if (maxDeviation <= 0.3)
        {
          MeatState = FRESH;
          LCD_moveCursor(0, 0);
          LCD_WriteString("T: ");
          LCD_WriteString(tempStr);
          LCD_WriteString(" H: ");
          LCD_WriteString(humidityStr);
        }
        else if (maxDeviation > 0.3)
        {
          MeatState = ROTTEN;
          LCD_moveCursor(0, 0);
          LCD_WriteString("T: ");
          LCD_WriteString(tempStr);
          LCD_WriteString(" H: ");
          LCD_WriteString(humidityStr);
        }
        MachineState = RELEASED;
      }
    }
    else if (MachineState == DB_PRESSED)
    {

      delayMs(50);
      MachineState = PRESSED;
    }
    else if (MachineState == DB_RELEASED)
    {

      delayMs(50);
      MachineState = RELEASED;
    }
  }
  return 0; // This will never be reached
}

ISR(INT5_vect)
{
  // it checks the state of the button to if it is in a release state itll go to the debouncing pressed state
  if (MachineState == RELEASED)
  {
    MachineState = DB_PRESSED;
    uart_transmit_string("button pressed");
    uart_transmit_string("\n");
  }
  else if (MachineState == PRESSED)
  {
    // it checks the state of the MachineState to if it is in a pressed state itll go to the debouncing release state
    MachineState = DB_RELEASED;
    uart_transmit_string("button pressed");
    uart_transmit_string("\n");
  }
}