/**
 * @file    pin_map.h
 * @author  Remington Bullis
 * @date    27 March 2019
 * @brief   This file contains functions declarations for using the L298 motor 
 *			driver.
 * 
 * @details	This file declares the L298 class and all the public and private 
 *			functions.
 *
 */


#ifndef PIN_DEFS_H
#define PIN_DEFS_H
/* 
>>> These are the ARDUINO pin mappings. <<<
#define US_RECV  12
#define US_ECHO  A4  
#define US_TRIG  A5 

#define H_A_EN 5
#define H_B_EN 6
#define H_IN1  7
#define H_IN2  8
#define H_IN3  9
#define H_IN4  11

#define IR_RECV 12

#define LED    13

#define LINE_R 10
#define LINE_M 4
#define LINE_L 2
*/

//<<< These are bit offsets for each pin, usable for PIN, DDR, PORT. >>>pu
/// Ultrasonic Echo pin
#define US_ECHO   4  // PORTC
/// Iltrasonic trigger pin
#define US_TRIG   5  // PORTC

/// H bridge motor enable A
#define H_A_EN    5  // PORTD
/// H bridge motor enable B
#define H_B_EN    6  // PORTD
/// H bridge input 1
#define H_IN1     7  // PORTD
/// H bridge input 2
#define H_IN2     0  // PORTB
/// H bridge input 3
#define H_IN3     1  // PORTB
/// H bridge input 4
#define H_IN4     3  // PORTB


/// Infrared receive pin
#define IR_RECV   6  // PORTB
///In built LED connected pin
#define LED       5  // PORTB

//#define LINE_R    2  // PORTB
//#define LINE_M    4  // PORTD
//#define LINE_L    2  // PORTD


#endif
