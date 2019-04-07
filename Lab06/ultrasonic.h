#ifndef __US
#define __US
	
	#include <avr/io.h>
	#include <util/delay.h>
 	#include "ultrasonic.h"

	void distance_init();
	void distance_trigger();
	int distance_receive();



#endif