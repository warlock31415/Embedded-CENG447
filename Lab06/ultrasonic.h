/**
 * @file    ultrasonic.h
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This file contains functions declarations for using the ultrasonic sensor
 *
 */
#ifndef __US
#define __US
	
	#include <avr/io.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>

	void distance_init();
	void distance_trigger();
	int distance_receive();

#endif