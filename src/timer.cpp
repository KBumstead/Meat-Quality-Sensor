// Author: 
// Net ID: 
// Date: 
// Assignment: Lab 3
//----------------------------------------------------------------------//

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
	
//initizializing timer 1 and set it to ctc mode
  TCCR1A &= ~(1 << WGM10);
  TCCR1A &= ~(1 << WGM11);
  TCCR1B |= (1 << WGM12);
  TCCR1B &= ~(1 << WGM13);

  // set timer 1 prescaler to 8
  TCCR1B &= ~(1 << CS10);
  TCCR1B &= ~(1 << CS12);
  TCCR1B |= (1 << CS11);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    int prescaler = 8; //setting variable prescaler to 8 for later calculations

    int ticks = ((0.000001 * 16000000) / prescaler) - 1; //formula to count ticks to reach 1 mikro second

    //or1ah is 16 bits so we need to set to 2 variables 
    OCR1AH = ticks >> 8; //upper 8 bit tick count
    OCR1AL = ticks; //lower 8 bit tick count

    for (unsigned int i = 0; i < delay; i++) {
        TCNT1 = 0;//to reset the counter in each loop to make sure it stays 0
        while (!(TIFR1 & (1 << OCF1A))){}; //loop to wait until the comparison if its both 1 then out of the loop
        TIFR1 |= (1 << OCF1A);             // clear the ocf1a flag
    }
}


/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
 //initialize timer 0 and set it to ctc mode
    //initialize timer 0 and set it to ctc mode
    TCCR0A &= ~(1 << WGM00); //to clear the wgmo
    TCCR0A |= (1 << WGM10); //put 1 in wgm10 to set it to mode 2 which is ctc mode 2 010

    //to set prescaler to 64
    TCCR0B |= ((1 << CS01) | (1 << CS00));
    TCCR0B &= ~((1 << CS02) | (1 << WGM02));
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
   //because it is ctc mode, had to set our own count register which results in 249
    //by counting the OCR0A
    int prescaler = 64;
    OCR0A = ((0.001 * 16000000) / prescaler) - 1;
    
    for (unsigned int i = 0; i < delay; i++)
    {

        TCNT0 = 0;//to reset the counter in each loop to make sure it stays 0
        while (!(TIFR0 & (1<<OCF0A))) //loop to wait for the comparison if its both 1 then we go out from the loop
            ;
        TIFR0 |= (1 << OCF0A); //once the loop above finish tifr0 becomes 1 to show that the comparison has been reached.
    }
}