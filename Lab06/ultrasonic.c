#include "ultrasonic.h"
#include "pin_map.h"
#include "serial.h"
#include <stdio.h>
#include<avr/io.h>
#include <avr/interrupt.h>

	volatile int timer;
 	volatile int flag;


void distance_init(){

	TCCR1B |= (1<<CS11); // Prescaler clk/8
	DDRC |= (1<<US_TRIG); //set pinmode(output)

	PCICR |= (1<<PCIE1);
	PCIFR &= ~(1<<PCIF1);
	PCMSK1 |= (1<<PCINT12);
}

void distance_trigger(){
	sei();
	//timer =0;
	TCNT1 = 0;
	PORTC |= (1<<US_TRIG);
	while(TCNT1 < 20){	}
	PORTC &= ~(1<<US_TRIG);
}

int distance_receive(){
		loop_until_bit_is_set(PINC,US_ECHO);
		loop_until_bit_is_clear(PINC,US_ECHO);
	return timer/116;
}

ISR(PCINT1_vect){
	cli();
	if (flag == 0)
	{
		TCNT1=0;
		flag = 1;
	}
	else {
		timer = TCNT1;
		flag=0;
	}
	sei();
}




 
