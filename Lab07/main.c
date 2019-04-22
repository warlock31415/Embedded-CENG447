/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    22 April 2019
 * @brief   This is the main file for Lab 6.
 *
 * @details This is the entrypont for the code. It calls all the other functions
 *			to get the robot moving. In this case it implements a pid loop that
 *			has the robot maintian a certain distance from an object.
 */

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 7 for the CENG447: Embedded
 *			systems classes for the Spring of 2019.
 *
 *          Servos are used in a variety of applications for actuation 
 *			purposes. Hobby servos are a cheap way to add motion to projects
 *			that require precise positioning and not much torque. Such as 
 *			actuating the pylons on a model airplane or waving hello using a 
 *			robot hand. The servos being used in the lab are micro servos and 
 *			cover a 0-180 degree arc. The standard micro servos require a 50Hz 
 *			signal with a $\approx$ 1-2ms on time. Since timers 0 and 1 on the 
 *			ATMEGA328P are taken up by the wheels and ultrasonic respectively 
 *			the only way to achieve this is by using the 8-bit Timer/counter2. 
 *			As always the source code can be found 
 *          <a href="https://github.com/warlock31415/Embedded-CENG447/tree/master/Lab07">here</a>
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


#include<avr/io.h>
#include<avr/interrupt.h>
#include "servo.h"
#include<util/delay.h>
#include "ultrasonic.h"
#include "serial.h"


 int main()
{

	int i=0;
	servo_init();
	distance_init();
	ioinit();
	while(1)
	{
		for(i=0;i<=180;i++){
			printf("Position:%d",i);
			servo_position(i);
			printf(" sensor value: %d\n",distance_receive());		
		}
	servo_position(90);
	_delay_ms(1000);
	servo_position(180);
	_delay_ms(1000);
	servo_position(0);
	_delay_ms(1000);
	}
	return 1;
}