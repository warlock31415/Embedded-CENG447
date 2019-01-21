/**
 * @file    blink.c
 * @author  Pratik Kunkolienker
 * @date    21 Jan 2019
 * @brief   This program is proof that the correct environment was set up to work with the ATMEGA328p
 *
 * @details This program was made to run on the ATMEGA328p running at 16MHz. The program is set to
 *          blink the built-in LED connected to pin 13 at an interval of 1 second. This can be modified
 *          by changing the value of the t variable in the main function. In addition, if a LED is
 *          connected to pin 8 via a current limiting resistor (1k in my case) the LEDs will flash in
 *          an alternating manner.
 */

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 0 for the CENG447: Embedded systems classes for the Spring of 2019.
 *
 *          The <a href="blink.c">source file</a> was compiled using avr-gcc and flashed on the ATMEGA328p using the AVRDUDE utility and the arduino
 *          UNO as a programmer.
 *
 * @section Image
 *          Link to the video <a href="https://youtu.be/9pCdGN9gtTw">https://youtu.be/9pCdGN9gtTw</a>
 *          @image html environment.png width=1000
 *
 * @section Makefile
 *          Put the <a href="../../makefile">makefile</a> in the src directory. Change the variables at the start to suit your needs.
 *
 * @section Issues
 *          -# Using Make
 *            - I have never written a makefile before and spent a long time understanding and writing my own makefile.
 *          -# Uploading Code
 *            - It was noticed that the programmer would time out if the Bluetooth module was plugged in.
 *
 */

#include <avr/io.h>
#include <util/delay.h>

void time_delay(int t);


/**
* @details  This is the main function. It sets up PORTB registers to
*           output and handles the blink logic
* @returns  void
**/
int main(void){
  int t = 1; // set delay time in seconds
  DDRB = 0xFF; //Set all register on PORT B as output

  PORTB ^= 0b00000001; // Offset pin 8 to achieve alternating LED flashes
  while(1){
    time_delay(t); // Loop for t amount of seconds

    PORTB ^= 0b00100001; // XOR assignment which toggles pins 13 and 8

  }
  return 0;
}

/**
* @details    This function takes in the time to wait in seconds and calls the _delay_ms()
*             macro as many times as needed.
* @param[in]   t time in seconds for delay
* @returns    void
**/
void time_delay(int t){
  for(int i = t*1000;i--;){
    _delay_ms(1); /* Needs to be called multiple time since
                      max is 262.14ms/F_CPU in MHZ        */
  }
}
