#define F_CPU 16000000

#include "serial.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

int main()
{

	ioinit();
	char Num;

	while(1)
	{
		printf("Enter a number:");
		scanf("%c",&Num);

		printf("Your number is : %c \n\r",Num);
		_delay_ms(500);
	}


}

