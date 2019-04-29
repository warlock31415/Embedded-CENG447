/**
 * @file    L298.c
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This file contains functions for using the L298 motor driver
 *
 */




#include "L298.h"
#include "pin_map.h"
#include <avr/io.h>
#include <util/delay.h>



/**
* @details  This function initializes Timer0 by setting the compare match 
*			registers A and B to clear on compare match. Setting the 
*			Waveform generation mode bits 1 and 0 and the PWM clock frquency.
*			This function also sets appropriate pins on PORTD as output.
* @returns  void
**/

void motor_init(char clk){
	TCCR0A |= (1<<COM0A1); //Clear OC0A on compare match
	TCCR0A |= (1<<COM0B1); //Clear OC0B on compare match

	TCCR0A |= ((1<<WGM01)|(1<<WGM00)); // Fast PWM mode 3

	TCCR0B |= clk;//(1<<CS02)|(0<<CS01)|(0<<CS00); 

	OCR0A = map(0);
	OCR0B = OCR0A;

	DDRD |= (1<<H_A_EN);
	DDRD |= (1<<H_B_EN);

	TCNT0 = 0;


}

/**
* @details  	This is a private function which is only available to functions 
*				inside the class. It takes a percentage of speed as input 100 
*				being max and 0 being the minimum and maps it from 0 to 255.
* @param [in] 	duty_cyc Percentage of max speed
* @returns  char duty_cyc*255/100
**/
 char map(int duty_cyc)
{
	return duty_cyc*255/100;
}

/**
* @details  This function sets the direction of the motors such that the left
*		side motors turn forwards and the right side mmotors turn the opposite
*		direction. The function calls the \a map() such that a percentage is 
*		mapped to a 0-255 range.
*@param [in] speed A percentage of max speed 0-100%
* @returns  void
**/
void turn_right(int speed){

	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN3);
	PORTB &= ~(1<<H_IN4);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

/**
* @details  This function sets the direction of the motors such that the both
*		run forwards. The function calls the \a map() such that a percentage is 
*		mapped to a 0-255 range.
*@param [in] speed A percentage of max speed 0-100%
* @returns  void
**/
void forward(int speed){
	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

/**
* @details  This function sets the direction of the motors such that the right
*		side motors turn forwards and the left side mmotors turn the opposite
*		direction. The function calls the \a map() such that a percentage is 
*		mapped to a 0-255 range.
*@param [in] speed A percentage of max speed 0-100%
* @returns  void
**/
void  turn_left(int speed){
	PORTD &= ~(1<<H_IN1);
	PORTB |= (1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3);

	OCR0A = map(speed);
	OCR0B =OCR0A;
}

/**
* @details  This function sets the direction of the motors such that the both
*		run backwards. The function calls the \a map() such that a 
*		percentage is mapped to a 0-255 range.
*@param [in] speed A percentage of max speed 0-100%
* @returns  void
**/
void back(int speed){
	PORTD &= ~(1<<H_IN1);
	PORTB |= (1<<H_IN2); 
	PORTB &= ~(1<<H_IN4);
	PORTB |= (1<<H_IN3);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

/**
* @details  Calls the \a turn_right() function 4 times such that the robot 
*			traces a square whose side length is as long as 3s in equivalent
*			distance. The spedd is set to 0 at the end.
*@param [in] speed A percentage of max speed 0-100%
* @returns  void
**/
void square_turn(int speed){
	forward(speed);
	_delay_ms(1000);
	turn_right(speed);
	_delay_ms(1000/2);
	forward(speed);
	_delay_ms(1000);
	turn_right(speed);
	_delay_ms(1000/2);
	forward(speed);
	_delay_ms(1000);
	turn_right(speed);
	_delay_ms(1000/2);
	forward(speed);
	_delay_ms(1000);
	motor_init(0);

}

/**
* @details  Sets the right side speed a little faster than the left speed
* 			so that the robot traces a circle. The radius of the circle is 
*			a relationship of the difference between the wheel speeds.
* @returns  void
**/
void circ()
{
	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3); 

	OCR0A = map(100); 
	OCR0B = map(30);
}

void left_side(int speed)
{
	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2);

	OCR0A = map(speed);
}

void right_side(int speed){
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3); 

	OCR0B = map(speed);
}