#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"
#include <string.h>
#include "timer.h"

void initUART()
{
    unsigned int ubrr = 103;                // UBRR = (F_CPU / 16 / baud_rate) - 1 for 9600 baud with 16 MHz clock
    UBRR0H = (unsigned char)(ubrr >> 8);    // Set high byte of baud rate
    UBRR0L = (unsigned char)ubrr;           // Set low byte of baud rate
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);   // Enable transmitter and receiver
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, 1 stop bit
}

// Send a character over UART
void uart_transmit_char(char c)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;     // Wait for the transmit buffer to be empty
    UDR0 = c; // Transmit the character
}

// Send a string over UART
void uart_transmit_string(const char *str)
{
    while (*str)
    {
        uart_transmit_char(*str++);
    }
}

// Receive a byte over UART
char uart_receive_char()
{
    while (!(UCSR0A & (1 << RXC0)))
        ;                 // Wait for data to be received
    return UDR0; // Get and return received data from the buffer
}

// Receive a string over UART (max length 32 for simplicity)
void uart_receive_string(char *buffer, uint8_t max_length)
{
    uint8_t index = 0;
    while (1)
    {
        buffer[index] = uart_receive_char();  // Receive a character
        uart_transmit_string("Iteration: ");
        uart_transmit_char('0' + index);  // Print the current iteration index
        uart_transmit_string(" Character: ");
        uart_transmit_char(buffer[index]);  // Print the received character
        uart_transmit_string("\r\n");

        // If we receive a newline or carriage return, treat it as the end of the message
        if (buffer[index] == '\n' || buffer[index] == '\r')
        {
            buffer[index + 1] = '\0';  // Null-terminate the string
            break;
        }
        index++;
    }

    // If response length is shorter than expected, print the buffer content anyway
    uart_transmit_string("Response: ");
    uart_transmit_string(buffer);
    uart_transmit_string("\r\n");
}


void check_connection_state()
{
    // Send AT+STATE command
    uart_transmit_string("AT+VERSION\r\n");
    delayMs(2000);  // Wait 2 seconds for the response

    // Wait for the response and process it
    char response[100]; // Buffer for storing the response
    uart_receive_string(response, sizeof(response));

    uart_transmit_string("\r\nRaw response: ");
    uart_transmit_string(response);
    uart_transmit_string("\r\n");
}
