/**
 * @file   servo.h
 * @author  Pratik Kunkolienker
 * @date    22 April 2019
 * @brief   This is the main file for Lab 6.
 *
 * @details This is the header file for the servo library. Change the max and 
 *			min servo values here as required.
  */
///Checks if the required dependancies were included
#ifndef SERVO_h
#define SERVO_H

	#include<avr/io.h>
	///Minimum allowable OCR2B value
	#define SERVO_MIN 5.0
	///Maximum permissible OCR2B value. If 19 is used a noticeable voltage drop
	///is seen.
	#define SERVO_MAX 18.0

	void servo_init();
	void servo_position(uint8_t angle);
#endif