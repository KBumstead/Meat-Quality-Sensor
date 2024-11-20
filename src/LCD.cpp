// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
  //below to initialize all pins on the arduino for lcd pin d4 to d7
  DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3);
  //below to intiazlise pins for the Enable pin and RS pin respectively
  DDRB |= (1 << DDB4) | (1 << DDB6);
}



/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
  // PORTA = data;
  //below to command port for the lcd that is in pot a0,a1,a2,a3
  PORTA = (PORTA & 0xf0) | (data & 0x0f);
  PORTB &= ~(1 << PORTB6); //set RS to low which means its taking command data input
  PORTB |= (1 << PORTB4); //enable to lcd 
  delayUs(1);//delay
  PORTB &= ~(1 << PORTB4); //disable the lcd
  delayUs(delay); //add another delay needed for the lcd process
}



/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
  //similar to 4bit command in this function its basically 2 4 bits combine into 8 bits
  //four bits  four bits 
   fourBitCommandWithDelay(command >> 4, delay);
   fourBitCommandWithDelay(command, delay);
}



/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
  //set RS as high so it becomes text data input
  PORTB |= (1 << PORTB6);

  //make the upper 4 bits of the character
  PORTA = (PORTA & 0xF0) | ((character & 0xF0) >> 4);
  PORTB |= (1 << PORTB4); //turn LCD E to high
  delayUs(1);
  PORTB &= ~(1 << PORTB4); //turn LCD E to low

  //make the lower 4 bits of the character
  PORTA = (PORTA & 0xF0) | (character & 0x0F);
  PORTB |= (1 << PORTB4); // turn LCD E to high
  delayUs(1); // delay 1 us
  PORTB &= ~(1 << PORTB4); // turn LCD E to low
  
  //added delay
  delayUs(46);
}




/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
  //loop for writeCharacter function to show the string
  while (*string != '\0') {
    //write character function to write the character
    writeCharacter(*string);
    //string++ for the next character in the while loop
    string++;
  }
}



/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
  //the countings of ddram address
  //0x80 sets the d7 as 1  
  //the bottom y and 0x0F for the lower 4 bits to set the cursors horizontal position
	unsigned char ddram = 0x80 + (y & 0x0F);  

  if (x == 1) ddram += 0x40; //if x equals to 1 then the cursor will be in the 2nd line set db6 as 1
  eightBitCommandWithDelay(ddram, 53); //to execute the command
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 15 milliseconds
  delayMs(15);
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(0b0011, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(0b0011, 100); 
  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay
  fourBitCommandWithDelay(0b0011, 100);
  // write 0b0010 to DB[7:4] and 100us delay.
  fourBitCommandWithDelay(0b0010, 100);
  // Function set in the command table with 53us delay
  eightBitCommandWithDelay(0b00101000, 53);
  // Display off in the command table with 53us delay
  eightBitCommandWithDelay(0b00001000, 53);
  // Clear display in the command table. Remember the delay is longer!!!
  eightBitCommandWithDelay(0b00000001, 3000);
  // Entry Mode Set in the command table.
  eightBitCommandWithDelay(0b00000110, 1000);
  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  eightBitCommandWithDelay(0b00001100, 1000);

}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins(); // initialize the ddr function
  initLCDProcedure(); //initialize the LCD procedure basically setting up the lcd to work
} 