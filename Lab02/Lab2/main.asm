.equ F_CPU=16000000
.equ BAUDRATE = 9600
.equ BAUD_PRESCALE = F_CPU/(16*BAUDRATE)-1 ;Pg 179

.def out_buf = r16
.def uart_buf = r18
.def uart_buf2 = r19


;Initialize Stack pointer
PUSH r23
LDI	r23, low(RAMEND)
OUT	SPL, r23
LDI	r23, high(RAMEND)
OUT	SPH, r23
POP r23

;Initialize UART 
LDS uart_buf,UCSR0B
LDI uart_buf,(1<<RXEN0)|(1<<TXEN0) ;Enable Rx and Tx
STS UCSR0B,uart_buf 

LDS uart_buf,UCSR0C
ORI uart_buf,0b00000110
STS UCSR0C,uart_buf
;Bits 7:6 - Select Async or Sync
;Bits 5:4 - Disable Parity
;Bits 3	  - 1 Stop bit
;Bits 2:1 
;+Bit 2 in UCSR0B 
;		  - 8 bits of data
;Bits 0   - Only used for synchronus operations

LDI uart_buf,high(BAUD_PRESCALE)
LDI uart_buf2,low(BAUD_PRESCALE)
STS UBRR0H,uart_buf
STS UBRR0L,uart_buf2

CLR out_buf

Main:
	LDS r17,UCSR0A
	SBRC r17,RXC0 ;Check if Receive was completed
		RCALL Uart_rx
	ORI r17,(1<<TXC0) ;Flush the transmit buffer
	SBRC r17,UDRE0    ;Check if the transmit buffer is empty
		RCALL Uart_tx  ;Write to UDR0 if UDRE0 is set
	RCALL Led_on
	RCALL Delay
	RCALL Led_off
	RCALL DELAY
	RJMP Main

Uart_rx:
	LDS out_buf,UDR0
	RET

Uart_tx:
	STS UDR0,out_buf
	RET
	
Delay: 
	LDI r21,0x01
	LDI r22,0x04
	LDI r23,0x53
	Delay_loop:
		DEC r21     ; 1 clk cycle
		BRNE Delay_loop ; 2 clk cycles (1 if true)
		DEC r22    ; 1 clc cycle
		BRNE Delay_loop ; 2 clk cycles
		DEC r23
		BRNE Delay_loop
	RET  
	

Led_on:
	PUSH r17
	LDI r17,0x07 ;Set first 3 bits of DDRB to output
	OUT DDRB,r17 ;
	OUT PORTB,out_buf ;Write the pressed number to LEDs
	POP r17
	RET

Led_off:
	PUSH r17
	CLR r17
	OUT PORTB,r17 ;Clear all pins on PORTB
	POP r17
	RET