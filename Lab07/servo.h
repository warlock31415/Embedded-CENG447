#ifndef SERVO_h
#define SERVO_H

	#include<avr/io.h>
    #include "pin_map.h"

	#define SERVO_MIN 5.0
	#define SERVO_MAX 18.0

	void servo_init();
	void servo_position(uint8_t angle);
#endif