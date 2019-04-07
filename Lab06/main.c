#include "serial.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "ultrasonic.h"
#include<avr/interrupt.h>
#include "pin_map.h"





int main()
{
	int distance;
	ioinit();
	sei();
	distance_init();

	while(1){
		_delay_ms(250);
		distance_trigger();
		distance = distance_receive();
		printf("timer=%d\r\n",distance);
	}
}

