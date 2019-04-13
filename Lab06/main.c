/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This is the main file for Lab 6.
 *
 * @details This is the entrypont for the code. It calls all the other functions
 *			to get the robot moving. In this case it implements a pid loop that
 *			has the robot maintian a certain distance from an object.
 */

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 6 for the CENG447: Embedded
 *			systems classes for the Spring of 2019.
 *
 *          Lab 6's goal was use Timer1 to count the duration of the pulse width
 *			of the echo pulse on the ultrasonic sensor and to generate an 
 *			accurate 10ms trigger signal. As always the source code can be found 
 *			<a href="https://github.com/warlock31415/Embedded-CENG447/tree/master/Lab06">here</a>
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
#include "ultrasonic.h"
#include<avr/interrupt.h>
#include "pin_map.h"
#include "L298.h"

/// P gain of the PID controller
#define kp 2

/// I gain of the PID controller
#define ki 0

// D gain of the PID controller
#define kd 1

// Set point for the PID loop
#define set_point 8

void pid();





int main()
{
	int distance;
	//ioinit();
	sei();
	distance_init();
	motor_init(5);

	while(1){
		_delay_ms(250);
		pid();
	}
}


 /**
 * @details This function handles the pid control. The function makes
 			sure that the robot stays at the set point.
 * @returns void
 *
 *
 */
void pid(){
	unsigned short integral =0;
	unsigned short derivative =0;
	unsigned short last_error =0;
	char pwm;
	distance_trigger();
	int current_position = distance_receive();
	while(current_position!=set_point){

		distance_trigger();
		current_position = distance_receive();
		int error = -(set_point-current_position);

		integral = integral+error;

		derivative = error - last_error;

		pwm = (kp*error)+(ki*integral)+(kd*derivative);
		//printf("pwm=%d -> distance = %d\n",pwm,current_position);

		if(pwm>100) pwm = 100;
		else if (pwm<-100) pwm = -100;


		if(pwm>0){
			forward(pwm);
		}
		else if (pwm<0)back(pwm);

		else motor_init(0);

		last_error = error;
		//_delay_ms(250);
	}



}