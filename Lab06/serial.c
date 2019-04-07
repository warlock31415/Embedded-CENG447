#include "serial.h"
#include <avr/io.h>
#include <stdio.h>

void ioinit()
{
	DDRD = 0b11111010;

	UBRR0H = ((MYUBRR)>>8);
	UBRR0L = MYUBRR;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	stdout = &mystdout;
	stdin = &mystdin;
}

static void uart_putchar(char c, FILE *stream)
{
	if(c=='\n') uart_putchar('\r',stream);
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = c;

}

char uart_getchar(void)
{
    while(!(UCSR0A &(1<<RXC0)));
    return(UDR0);
}

