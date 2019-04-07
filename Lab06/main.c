#include "serial.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include<avr/interrupt.h>

volatile int timer =0;



int main()
{

	ioinit();
	sei();
	distance_init();

	while(1){
		_delay_ms(250);
		distance_trigger();
		distance_receive();

		int distance = timer/116;
		printf("timer=%d\r\n",distance);
	}
}

ISR(PCINT1_vect){
	cli();
	TCNT1=0;
}