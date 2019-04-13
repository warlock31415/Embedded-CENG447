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
 *			A clk/8 prescaler was used to get a tick every 0.5ms. This meant 
 *			that in order to generate a 10ms pulse we'd have to count 20 ticks.
 *			To count the pulse width of the return pulse, a pin change interrupt
 *			was used. The pin change interrupt triggers on both falling and
 *			rising edges so accurate track must be kept of which event caused
 *			interrupt. A flag was used to do this. On a rising edge, the flag is
 *			set to 1 and TCNT1 is set to 0. On a falling edge, the flag is 
 *			cleared and the vlaue in TCNT1 is read.
 *			
 *			While this is happening, The program first waits for the ECHO pin to
 *			read high and then waits for the pin to go low again (The interrupt
 *			occures during this wait). It then returns the value that was read 
 *			from TCNT1 and divides it  by 116. 

 *			The division factor was obtained from the datasheet of the sensor. 
 *			But since every tick is 0.5ms we must first divide it by 2 to get
 *			the time elapse in ms and then divide it by 58 (datasheet value) to
 *			get the distance.
 *			
 *
 * @section Video
 * The robot can be seen measuring distances 
 * <a href="https://photos.app.goo.gl/2ECBMUaLxQM8WiUg8">here</a>
 *
 * The robot can be seen keep it's distance from the wall 
 *	<a href="https://photos.app.goo.gl/sDKsXncgCjGFD5kz8">here</a> 			
 * @section Document
 * Download the PDF by clicking <a href="./Lab_6.pdf"> here</a>
 *
 * @section Issues
 *-# Broken US
 *    + The initial ultrasonic sensor was broken and had to replaced. 
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

/// D gain of the PID controller
#define kd 1

/// Set point for the PID loop
#define set_point 8

void pid();


/**
* @details initializes the timer and the motor controller
* @returns void
**/


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