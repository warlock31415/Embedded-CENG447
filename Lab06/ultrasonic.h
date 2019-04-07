#ifndef __US
#define __US
	
	#include <avr/io.h>
	#include <util/delay.h>
 	#include "ultrasonic.h"

	void distance_init();
	void distance_trigger();
	int distance_receive();

	extern volatile int timer;
	extern volatile int flag;

#endif