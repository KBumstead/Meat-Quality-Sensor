#include <DHT.h>
#include <Arduino.h>

// Pin configuration
#define DHTPIN 2      // Digital pin where the DHT sensor is connected
#define DHTTYPE DHT22 // DHT 22 (AM2302)

#define MAXTIME 85 // Timeout for data reading

// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

int main()
{
  // Arduino-specific initialization
  init();             // Initialize Arduino core
  Serial.begin(9600); // Initialize Serial communication at 9600 baud
  dht.begin();        // Initialize the DHT sensor

  while (1) // Infinite loop
  {
    // Read temperature from the DHT sensor
    float temperature = dht.readTemperature();

    if (isnan(temperature))
    {
      Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");
    }

    delay(2000); // Wait for 2 seconds between readings
  }

  return 0; // This will never be reached
}
