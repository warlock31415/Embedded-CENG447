#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBT F_CPU/16/BAUD-1