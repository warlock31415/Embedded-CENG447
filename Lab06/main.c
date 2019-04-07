#define F_CPU 16000000

#include "serial.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

int main()
{

	ioinit();
	uint8_t Num;

	while(1)
	{
		printf("Enter a number:");
		scanf("%d",&Num);

		printf("Your number is : %d \n\r");
		_delay_ms(500);
	}


}

