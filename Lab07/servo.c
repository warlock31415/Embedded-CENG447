/**
 * @file    servo.c
 * @author  Pratik Kunkolienker
 * @date    22 April 2019
 * @brief   This is the main file for Lab 6.
 *
 * @details This file contains the required functions for the servo to work
 */

#include "servo.h"
#include "pin_map.h"

static uint8_t map(uint8_t angle);


/**
* @details Initializes Timer/counter2 to phase corrected PWM mode at 1024
*			prescaler and sets output compare registers A and B to 156 and 16 
*			respectively.		
* @returns void
**/
void servo_init(){
	DDRB = 0xFF;
	PORTB = 0xFF;
	DDRD = 0xFF;

	TCCR2A = 0xA1;
	TCCR2B = 0x0F;

	TCNT2=0;

	OCR2A = 156;
	OCR2B =16;
}

/**
* @details Checks to see if the position supplied is valid and sets default 
* 		   values if not.
* @param [in] angle Specifies the servo position in degrees
* @returns void
**/
void servo_position(uint8_t angle){
	if (angle<0) angle =0;
	if (angle>180) angle =180;
	OCR2B = map(angle);

}


/**
* @details Maps the input angle to the corresponding OCR2B value
* @param [in] angle Specifies the servo position in degrees
* @returns uint8_t
**/
static uint8_t map(uint8_t angle){
	double servo_angle = (angle)*(SERVO_MAX-SERVO_MIN)/180+SERVO_MIN;
	if (servo_angle-(int)servo_angle>=0.5){
		servo_angle = servo_angle+0.5;
	}
	return servo_angle;
}