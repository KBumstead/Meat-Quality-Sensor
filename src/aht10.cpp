// #include <avr/io.h>
// #include "i2c.h"
// #include <Arduino.h>

// #define AHT10_ADDRESS 0x38 // AHT10 I2C address

// // Initialize I2C
// void initI2C()
// {
//     DDRD |= (1 << DDD0) | (1 << DDD1);      // Set SDA (PD0) and SCL (PD1) as output
//     PRR0 &= ~(1 << PRTWI);                  // Enable I2C power
//     TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); // Set prescaler to 1
//     TWBR = 0xC6;                            // Set bit rate for 100kHz
//     TWCR |= (1 << TWINT) | (1 << TWEN);     // Enable I2C
// }

// // Start I2C transfer
// void startI2C_Trans(char SLA)
// {
//     TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Start condition
//     while (!(TWCR & (1 << TWINT)))
//         ; // Wait for start condition

//     TWDR = (SLA << 1);                 // Load slave address
//     TWCR = (1 << TWINT) | (1 << TWEN); // Clear TWINT and enable TWI
//     while (!(TWCR & (1 << TWINT)))
//         ; // Wait for transmission to complete
// }

// // Stop I2C transfer
// void stopI2C_Trans()
// {
//     TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Stop condition
// }

// // Write data to I2C
// void write(unsigned char data)
// {
//     TWDR = data;                       // Load data to be written
//     TWCR = (1 << TWINT) | (1 << TWEN); // Clear TWINT and enable TWI
//     while (!(TWCR & (1 << TWINT)))
//         ; // Wait for data to be transmitted
// }

// // Read data from I2C
// unsigned char read_data()
// {
//     return TWDR; // Return received data from TWDR
// }

// // Read data from AHT10 (multiple bytes)
// void read_from(unsigned char SLA, unsigned char MEMADDRESS, unsigned char *data, unsigned char length)
// {
//     startI2C_Trans(SLA); // Start I2C transfer to slave
//     write(MEMADDRESS);   // Write memory address

//     TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Send repeated start for reading
//     while (!(TWCR & (1 << TWINT)))
//         ; // Wait for completion

//     TWDR = (SLA << 1) | 0x01;                        // Set slave address for reading (SLA + 1)
//     TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Trigger action, master acknowledges
//     while (!(TWCR & (1 << TWINT)))
//         ; // Wait for completion

//     // Read data
//     for (int i = 0; i < length - 1; i++)
//     {
//         TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // Continue reading and acknowledge
//         while (!(TWCR & (1 << TWINT)))
//             ;           // Wait for data
//         data[i] = TWDR; // Store received byte
//     }

//     // Final byte without acknowledgment
//     TWCR = (1 << TWINT) | (1 << TWEN); // Read final byte (no acknowledgment)
//     while (!(TWCR & (1 << TWINT)))
//         ;                    // Wait for completion
//     data[length - 1] = TWDR; // Store the last byte

//     TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Send stop condition
// }

// // Initialize AHT10 sensor
// void initAHT10()
// {
//     startI2C_Trans(AHT10_ADDRESS); // Start I2C transfer to AHT10
//     write(0xE1);                   // Initialization command
//     write(0x08);                   // Data byte
//     write(0x00);                   // Data byte
//     stopI2C_Trans();
//     delay(10); // Wait for sensor to initialize
// }

// // Read temperature and humidity from AHT10
// bool readAHT10(float *temperature, float *humidity)
// {
//     unsigned char data[6];

//     startI2C_Trans(AHT10_ADDRESS); // Start I2C transfer to AHT10
//     write(0xAC);                   // Trigger measurement command
//     write(0x33);                   // Data byte
//     write(0x00);                   // Data byte
//     stopI2C_Trans();
//     delay(100); // Wait for measurement to complete

//     // Read 6 bytes of data
//     read_from(AHT10_ADDRESS, 0x00, data, 6);

//     // Convert the raw data into temperature and humidity
//     uint32_t rawHumidity = ((uint32_t)(data[1] & 0x0F) << 16) | ((uint32_t)data[2] << 8) | data[3];
//     uint32_t rawTemperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

//     // Calculate humidity and temperature
//     *humidity = ((float)rawHumidity / 1048576.0) * 100.0;              // Convert to percentage
//     *temperature = ((float)rawTemperature / 1048576.0) * 200.0 - 50.0; // Convert to °C

//     return true;
// }

// void setup()
// {
//     Serial.begin(9600);
//     initI2C();   // Initialize I2C
//     initAHT10(); // Initialize AHT10 sensor
// }

// void loop()
// {
//     float temperature, humidity;

//     if (readAHT10(&temperature, &humidity))
//     {
//         Serial.print("Temperature: ");
//         Serial.print(temperature);
//         Serial.println(" °C");

//         Serial.print("Humidity: ");
//         Serial.print(humidity);
//         Serial.println(" %");
//     }
//     else
//     {
//         Serial.println("Failed to read from AHT10 sensor!");
//     }

//     delay(2000); // Wait 2 seconds before the next reading
// }
