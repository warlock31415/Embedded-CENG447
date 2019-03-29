#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define BAUDRATE 9600
#define PRESCALER F_CPU/(BAUDRATE*16UL)-1

int main(){
	UART_init();

	while(1){
	char x =receivebyte();
	sendbyte(7);
	
	}

	return 0;
}
