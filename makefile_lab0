# Change file name here 
TARGET=blink
# Change port
PORT = /dev/ttyACM0


CC = avr-gcc
CFLAGS = -g -Wall -Os
# Chage this if using a different chip also change frequency if required
DEVICE = atmega328p
FREQ = 16000000UL

AVRDUDE = avrdude
PROGRAMMER = arduino
OBJCOPY = avr-objcopy


all:$(TARGET).hex

# This is  the first dependancy, thus the base file name needs to be passed in (?)
$(TARGET).o: $(TARGET).c
	@ $(CC) $(CFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FREQ) -c $(TARGET).c

# The '%'' sign makes the Make file more generic, can be replaced by $(TARGET)
# $< is the dependancy and $@ is the target file
%.elf: %.o
	@ $(CC) $(CFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $< -o $@
	@ echo "Why are they called elf files???"

# Since the the rules are chained together Make deletes intermediate files
# automatically. Use ".PRECIOUS: %.elf" to let Make know to keep the intermediate
# files
.PRECIOUS: %.elf
# Removing the above line will execute a rm *.elf command automatically and thus
# make clean would throw up an error

%.hex: %.elf
	@ $(OBJCOPY) -j .text -j .data -O ihex $< $@

# Use avrdude to flash the chip
# Syntax : avrdude -c <programmer>
# 				-p <part #> -P <port>
#					-U flash\eeprom:w\r\v:<input file>
# use -b <baud rate> to over-ride baudrate
# http://www.ladyada.net/learn/avr/avrdude.html

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(DEVICE) -P $(PORT) -U flash:w:$<


clean:
	@ rm *.o *.elf

clean_all: clean
	@ rm *.hex
