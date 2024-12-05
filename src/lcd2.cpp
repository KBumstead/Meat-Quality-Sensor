#include <avr/io.h>
#include "timer.h"

#define F_CPU 16000000UL

// I2C Address of LCD (common: 0x27 or 0x3F)
#define LCD_I2C_ADDRESS 0x27

// Define SCL and SDA pins for ATmega2560
#define SCL PD0
#define SDA PD1

void init_i2c()
{
    PRR0 &= ~(1 << PRTWI); // by putting it to 0 we enable power to i2c to nyalain 2 wire serial interface

    // Set clock frequency for I2C (prescale and TWBR)
    TWSR = 0x00;                        // Prescaler value = 1
    TWBR = 72;                          // SCL frequency = 100kHz, assuming 16MHz clock
    TWCR |= (1 << TWINT) | (1 << TWEN); // enter MT mode to enable 2
}

void startI2C_Trans()
{
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); // Start condition
    while (!(TWCR & (1 << TWINT)))
        ; // Wait for TWINT flag set
}

void stopI2C_Trans()
{
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); // Stop condition
    while (TWCR & (1 << TWSTO))
        ; // Wait for stop to complete
}

void write(uint8_t data)
{
    TWDR = data;                       // Load data into data register
    TWCR = (1 << TWEN) | (1 << TWINT); // Start transmission
    while (!(TWCR & (1 << TWINT)))
        ; // Wait for TWINT flag set
}

void LCD_4BitCommand(uint8_t nibble, uint8_t control)
{
    uint8_t data = (nibble << 4) | control;
    startI2C_Trans();
    write(LCD_I2C_ADDRESS << 1); // Write mode
    write(data | 0x04);          // Enable high
    delayUs(1);
    write(data & ~0x04); // Enable low
    delayUs(100);        // Command delay
    stopI2C_Trans();
}

void LCD_8BitCommand(uint8_t byte, uint8_t control)
{
    LCD_4BitCommand(byte >> 4, control);   // Send upper nibble
    LCD_4BitCommand(byte & 0x0F, control); // Send lower nibble
}

void LCD_Command(uint8_t cmd)
{
    LCD_8BitCommand(cmd, 0x00); // RS = 0 for command
}

void LCD_Data(uint8_t data)
{
    LCD_8BitCommand(data, 0x01); // RS = 1 for data
}

void LCD_Init()
{
    delayMs(50); // Wait for LCD to power on

    // Initialize in 4-bit mode
    LCD_Command(0x33); // Initialize
    LCD_Command(0x32); // Set to 4-bit mode
    LCD_Command(0x28); // Function set: 4-bit, 2-line, 5x8 dots
    LCD_Command(0x0C); // Display on, cursor off
    LCD_Command(0x06); // Entry mode set: increment cursor
    LCD_Command(0x01); // Clear display
    delayMs(2);
}

void LCD_Clear()
{
    LCD_Command(0x01); // Clear display command
    delayMs(6);        // Clear command requires a delay of at least 1.52ms
}

void LCD_moveCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch (row)
    {
    case 0:
        address = 0x00;
        break; // Row 0
    case 1:
        address = 0x40;
        break; // Row 1
    case 2:
        address = 0x14;
        break; // Row 2
    case 3:
        address = 0x54;
        break; // Row 3
    default:
        address = 0x00;
        break; // Default to Row 0
    }

    address += col;              // Add column offset
    LCD_Command(0x80 | address); // Send Set DDRAM Address command
}

void LCD_WriteString(const char *str)
{
    while (*str)
    {
        LCD_Data(*str++);
    }
}