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
LDI	ZH, low(RAMEND)
OUT	SPL, ZH
LDI	ZH, high(RAMEND)
OUT	SPH, ZH

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

ldi r16,0x30
ldi r21,0x00

SIMULATE:
	ldi r22,(1<<RXC0)
	sts UCSR0A,r22
	clr r22
	ldi r22,0x06
	sts UDR0,r22

main:	
	rcall NUM_GEN ;Generate the TX number
	rcall UART_TX ;Send the Number

	lds r17, UCSR0A
	sbrc r17, RXC0
		rjmp UART_RX

	rcall BLINK
	rcall DELAY
	rcall UNBLINK
	;Reinitialize delay and num gen
	inc r16
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
	push r17
	lds r17,UCSR0A
	sbrs r17, RXC0
		rjmp UART_RX
	lds r21, UDR0
	pop r17
	ret

DELAY: 
	ldi XH,0x01
	ldi XL,0x04
	ldi YH,0x53
	DELAY_LOOP:
		dec XH     ; 1 clk cycle
		brne DELAY_LOOP ; 2 clk cycles (1 if true)
		dec XL     ; 1 clc cycle
		brne DELAY_LOOP ; 2 clk cycles
		dec YH
		brne DELAY_LOOP
	ret  
	

NUM_GEN:
	push r20
	cpi r16,0x38
	in  r20,SREG
	sbrs r20,2
		ldi r16,0x030
	pop r20
	ret

BLINK:
	push r17
	ldi r17,0xFF
	out DDRB,r17
	out PORTB,r16
	pop r17
	ret

UNBLINK:
	push r17
	clr r17
	out PORTB,r17
	pop r17
	rcall DELAY
	ret




	
