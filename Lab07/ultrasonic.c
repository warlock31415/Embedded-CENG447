
/**
 * @file    ultrasonic.c
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This file contains functions declarations for using the ultrasonic. 
 *
 */

#include "ultrasonic.h"
#include<avr/io.h>
#include <avr/interrupt.h>


/// Structure that contains the pulse length and the flag for PCINT1
struct timer_stat
	{
	// Stores the pulse 2*pulse length in ms
	volatile unsigned int timer;
	/// Checks the PCINT was triggered by a falling edge or a rising edge
 	volatile unsigned int flag;
 };

/// Initialized the echo status
 struct timer_stat stat = {0,0};



 /**
 * @details This function initializes Timer1 with a clk/8 prescaler. It also 
 *			 the US_TRIG pis as outpu on PORTC. Further, the function 
 *			 initializes pin change interrupt on the US_ECHO pin.
 * @returns void
 *
 *
 */
void distance_init(){

	TCCR1B |= (1<<CS11); // Prescaler clk/8
	DDRC |= (1<<US_TRIG); //set pinmode(output)

	PCICR |= (1<<PCIE1);
	PCIFR &= ~(1<<PCIF1);
	PCMSK1 |= (1<<PCINT12);
}


 /**
 * @details Sends a 10ms pulse to trigger the ultrasonic sensor.
 * @returns void
 */
void distance_trigger(){
	sei();
	TCNT1 = 0;
	PORTC |= (1<<US_TRIG);
	while(TCNT1 < 20){	}
	PORTC &= ~(1<<US_TRIG);
}

 /**
 * @details Waits for the pin to go high and then low. Once low get the amount
 *			of time elapsed and divide it by 116 (2*58)
 * @returns int distance
 *
 *
 */
int distance_receive(){
		distance_trigger();
		loop_until_bit_is_set(PINC,US_ECHO);
		loop_until_bit_is_clear(PINC,US_ECHO);
	return stat.timer/116;
}


 /**
 * @details Handles pin change interrupt on the US_ECHO pin. TCNT1 is set to 0 
 *			on the rising edge. TCNT1 is read on the falling edge. This assures
 *			accurate timing.
 * @returns void
 *
 *
 */
ISR(PCINT1_vect){
	cli();
	if (stat.flag == 0)
	{
		TCNT1=0;
		stat.flag = 1;
	}
	else {
		stat.timer = TCNT1;
		stat.flag=0;
	}
	sei();
}




 
