/**
 * @file    L298.h
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   This file contains functions declarations for using the L298 motor 
 *			driver.
 * 
 * @details	This file declares the L298	functions.
 *
 */
#ifndef L298_H
#define L298_H
	#include <avr/io.h>
	#include <util/delay.h>
	#include "pin_map.h"

	/**
		The L298 class object that has all the motor related functions
	**/
		void motor_init(char clk);
		void turn_right(int speed);
		void forward(int speed);
		void back(int speed);
		void turn_left(int speed);

		void square_turn(int speed);
		void circ();

		char map(int d_cyc);


#endif 