#include "ultrasonic.h"
#include "pin_map.h"
#include "serial.h"
#include <stdio.h>
#include<avr/io.h>
#include <avr/interrupt.h>

extern volatile int timer;


void distance_init(){

	TCCR1B |= (1<<CS11); // Prescaler clk/8
	DDRC |= (1<<US_TRIG); //set pinmode(output)

	//PCICR |= (1<<PCIE1);
	//PCIFR &= !(1<<PCIF1);
	//PCMSK1 |= (1<<PCINT12);
}

void distance_trigger(){
	//sei();
	//timer =0;
	TCNT1 = 0;
	PORTC |= (1<<US_TRIG);
	while(TCNT1 < 20){	}
	PORTC &= ~(1<<US_TRIG);
}

int distance_receive(){
	loop_until_bit_is_set(PINC,US_ECHO);
	TCNT1 = 0;
	loop_until_bit_is_clear(PINC,US_ECHO);
	timer = TCNT1;
	return timer/116;
}






 
