/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    22 April 2019
 * @brief   This is the main file for Lab 6.
 *
 * @details This is the entry point for the code. It initializes the required
 *			peripherals and the timers.
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
 *			signal with a approximately 1-2ms on time. Since timers 0 and 1 on the 
 *			ATMEGA328P are taken up by the wheels and ultrasonic respectively 
 *			the only way to achieve this is by using the 8-bit Timer/counter2. 
 *			As always the source code can be found 
 *          <a href="https://github.com/warlock31415/Embedded-CENG447/tree/master/Lab07">here</a>
 *			
 *			
 *
 * @section Video
 * Click
 * <a href="https://photos.app.goo.gl/kkpiPfTJJEt94HqN7">here</a> for the link
 * to the video
 *			
 * @section Document
 * Download the PDF by clicking <a href="./Lab_7.pdf"> here</a>
 *
 * @section Issues
 *-# Broken servo
 *    + The servo cracked during testing so had to be replaced. 
 */


#include<avr/io.h>
#include "servo.h"
#include<util/delay.h>
#include "ultrasonic.h"
#include "serial.h"


/**
* @details Initializes the Serial, ultrasonic sensor and the servo. Then does
*			a 0-180 degree sweep whilst taking distance measurements.And then 
*			goes to the centered, CCW and CW positions on the servo.
*			
* @returns void
**/


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