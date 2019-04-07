/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This is the main file for Lab 5. It declares the motor object 
 *			and calls the appropriate functions to complete the required task
 *
 * @details This program was written has two versions. One compiled in C that
 *			uses functions. The other, compiled in CPP and uses classes. The 
 *			main loop initializes the timer and calls the appropriate function
 *			such as circ and square turn.
 */

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 5 for the CENG447: Embedded
 *			systems classes for the Spring of 2019.
 *
 *          Lab 5's goal was use Timer0 to generate a PWM signal to run the 
 *			motors on the Elegoo robot using the LM298 motor controller module 
 *			and the provided header file. The source code can be found 
 *			<a href="https://github.com/warlock31415/Embedded-CENG447/tree/master/Lab05">here</a>
 *			
 *			The clock is set according to the following table:
 *			Set # 	|	Prescaler
 *			-------	|	-----------------------------------------
 *			0		|	No clock source
 *			1		|	No prescaler
 *			2		|	clk/8
 *			3		|	clk/64
 *			4		|	clk/256
 *			5		|	clk/1024
 *			6		| 	Ext clk on T0 pin. Clock on falling edge
 *			7		|	Ext clk on T0 pin. Clock on rising edge
 *
 *			The set # is passed into the init member function. This sets the 
 *			frequency of the PWM according to the table above.
 *
 * @section Video
 * Please follow <a href="https://photos.app.goo.gl/mHPKNQDd5JPLRGjX9">this link</a>
 * 			
 * @section Document
 * Download the PDF by clicking <a href="./Lab_5.pdf"> here</a>
 *
 * @section Issues
 *-# Half power
 *    + Since two motors are connected to the same output on the motor 
 *      controller output, the motors only get half the power each thus
 *      are never at full output.
 *-# Incorrect frequency
 *    + The motors make a whiney noise if too high of a frequency or too
 *		low a frequency is supplied
 */


#include <avr/io.h>
#include <util/delay.h>
#include "L298.h"

/**
 UART baudrate
 */
#define BAUDRATE 9600

/** 
UART baudrate prescaler
*/
#define PRESCALER F_CPU/(BAUDRATE*16UL)-1


/**
* Create a motor object that contains the initilization function and movement 
* functions
*/
L298 Motor;

/**
* @details Main function that uses the Motor object to complete the tasks
*			stated in the problem statement
* @returns void
**/
int main(){

	//Initialize motor with a fixed clock cycle
	Motor.init(5);

	//
	Motor.forward(100);
	_delay_ms(2000);
	Motor.back(50);
	_delay_ms(2000);
	Motor.turn_left(100);
	_delay_ms(2000);
	Motor.turn_right(80);
	_delay_ms(4000);
	Motor.square_turn(80);
	_delay_ms(2000);
	// Go in a circle
	Motor.circ();

	while(1){
	}

	return 0;
}
