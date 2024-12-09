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

  // init the switch
  initSwitchPD2();

  initUART();    // Initialize UART
  delayMs(1000); // Delay to allow JDY-31 to initialize
  dht.begin();   // Initialize the DHT sensor

  uint16_t adcValue;
  float rs_ro_ratio, ppm;
  sei();

  LCD_moveCursor(0, 0);
  LCD_WriteString("Press the button to activate");

  // Main loop
  check_connection_state();
  float temperature;
  float humidity;
  char tempStr[10];
  char humidityStr[10];
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
      // dht sensor activate
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
      uart_transmit_string("Temperature: ");
      Serial.println(temperature);
      uart_transmit_string("Humidity:");
      Serial.println(humidity);
      dtostrf(temperature, 4, 1, tempStr);  // Convert temperature to string (4 characters wide, 1 decimal place)
      dtostrf(humidity, 4, 1, humidityStr); // Convert humidity to string (4 characters wide, 1 decimal place)
      // mq1135 sensor activate and get the values
      adcValue = readADC(0);
      rs_ro_ratio = getRsRoRatio(adcValue);
      ppm = calculatePPM(rs_ro_ratio);
      uart_transmit_string("PPM:");
      Serial.println(ppm);
      LCD_Clear();
      if (humidity > 50)
      {
        if (ppm <= 0.6)
        {
          MeatState = FRESH;
          LCD_moveCursor(0, 0);
          LCD_WriteString("T: ");
          LCD_WriteString(tempStr);
          LCD_WriteString(" H: ");
          LCD_WriteString(humidityStr);
        }
        else if (ppm > 0.6 && ppm < 1)
        {
          MeatState = ROTTING;
          LCD_moveCursor(0, 0);
          LCD_WriteString("T: ");
          LCD_WriteString(tempStr);
          LCD_WriteString(" H: ");
          LCD_WriteString(humidityStr);
        }
        else if (ppm >= 1)
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

    delayMs(2000); // Add delay to avoid frequent readings
  }
  return 0; // This will never be reached
}

ISR(INT5_vect)
{
  // it checks the state of the button to if it is in a release state itll go to the debouncing pressed state
  if (MachineState == RELEASED)
  {
    MachineState = DB_PRESSED;
    Serial.print("button pressed");
    Serial.print(MachineState);
  }
  else if (MachineState == PRESSED)
  {
    // it checks the state of the MachineState to if it is in a pressed state itll go to the debouncing release state
    MachineState = DB_RELEASED;
    Serial.print("button pressed");
  }
}