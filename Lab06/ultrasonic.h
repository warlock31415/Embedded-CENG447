#ifndef __US
#define __US
	
	#include <avr/io.h>
	#include <util/delay.h>
 	#include "ultrasonic.h"

	void distance_init();
	void distance_trigger();
	void distance_receive();



#endif