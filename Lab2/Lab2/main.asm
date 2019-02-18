;
; Lab2.asm
;
; Created: 2/14/2019 3:03:10 PM
; Author : 7410983
;

.equ F_CPU = 16000000
.equ BAUDRATE = 9600
.equ BAUD_PRESCALE = F_CPU/(BAUDRATE*16)-1

.def out_buf = r16
.def uart_buf = r18
.def uart_buf2 = r19

;Initialize SP
LDI	r23, low(RAMEND)
OUT	SPL, r23
LDI	r23, high(RAMEND)
OUT	SPH, r23

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

	ldi out_buf,0x00

main:	
	lds r17, UCSR0A
	sbrc r17, RXC0
		rjmp UART_RX
	andi out_buf,0x07 ;Mask to convert ASCII 0-7 to binary
	rcall UART_TX ;Transmit over UART
	rcall BLINK ;Turn on LEDs
	rcall DELAY ;wait 1 second
	rcall UNBLINK ;Turn off LEDs
	rcall DELAY
	rcall main

UART_TX:
	push r17
	lds r17, UCSR0A
	sbrs r17, UDRE0 ; Skip if bit in register is set
		rjmp UART_TX
	sts UDR0, out_buf ;Store to data space
	pop r17
	ret

UART_RX:
	lds out_buf, UDR0
	ret

DELAY: 
	ldi r21,0x01
	ldi r22,0x04
	ldi r23,0x53
	DELAY_LOOP:
		dec r21     ; 1 clk cycle
		brne DELAY_LOOP ; 2 clk cycles (1 if true)
		dec r22    ; 1 clc cycle
		brne DELAY_LOOP ; 2 clk cycles
		dec r23
		brne DELAY_LOOP
	ret  
	

BLINK:
	push r17
	ldi r17,0x07 ;Set first 3 bits of DDRB to output
	out DDRB,r17 ;
	out PORTB,out_buf ;Write the pressed number to LEDs
	pop r17
	ret

UNBLINK:
	push r17
	clr r17
	out PORTB,r17 ;Clear all pins on PORTB
	pop r17
	ret




	
