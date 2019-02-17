;
; Lab2.asm
;
; Created: 2/14/2019 3:03:10 PM
; Author : 7410983
;

;#define F_CPU 16000000
#define  BAUDRATE , 9600
#define  BAUD_PRESCALE , F_CPU/(BAUDRATE*16)-1

#define out_buf = r16
#define  uart_buf = r18
#define uart_buf2 = r19

ldi XH,0x01
ldi XL,0x04
ldi YH,0x53

rcall UART_INIT 

rjmp main
RE_INIT:
	ldi r20,0x00
; Replace with your application code
main:
	cpi r20,0x08
	in r21,SREG
	sbrs r21,2
		rjmp RE_INIT

	;rcall UART_RX
	;rcall DELAY
	mov out_buf , r20
	rcall UART_TX

	inc r20
	rcall main

UART_INIT:
	push uart_buf
	push uart_buf2
	lds uart_buf,UCSR0B
	ldi uart_buf ,(1<<RXEN0)|(1<<TXEN0);|(1<<RXCIE0) Enables receive interrupt, TODO: figure out interrupt
	sts UCSR0B,uart_buf ;Set bits 4 and 3, these enable TX and RX respectively

	lds uart_buf,UCSR0C
	ori uart_buf,0x06
	sts UCSR0C,uart_buf ; 8 bits with 1 end bit in asynchronus mode

	ldi uart_buf ,high(BAUD_PRESCALE)
	ldi uart_buf2, low(BAUD_PRESCALE)
	sts UBRR0H,uart_buf
	sts UBRR0L,uart_buf2
	pop uart_buf2
	pop uart_buf
	ret 

UART_TX:
	push r17
	lds r17, UCSR0A
	sbrs r17, UDRE0 ; Skip if bit in register is set
	rjmp UART_TX
	sts UDR0, out_buf ;Store to data space
	pop r17
	ret

UART_RX:
	push r17
	lds r17,UCSR0A
	sbrs r17, RXC0
	rjmp UART_RX
	lds out_buf, UDR0
	pop r17
	ret

DELAY: 
	dec XH     ; 1 clk cycle
	brne DELAY ; 2 clk cycles (1 if true)
	dec XL     ; 1 clc cycle
	brne DELAY ; 2 clk cycles
	dec YH
	brne DELAY
	ret  
	; Takes (XH*3+2)+(((XH*3+2)+3)*XL+2)+(((((XH*3+2)+3)*XL+2)+3)*YH+2) clock cycles
