# Change file name here 
TARGET=main
# Change port
PORT = /dev/ttyACM0
DEPENDENCIES = ultrasonic.c servo.c serial.c

CC = avr-gcc
CFLAGS = -g -Os -I./ 
# Chage this if using a different chip also change frequency if required
DEVICE = atmega328p
FREQ = 16000000UL

AVRDUDE = avrdude
PROGRAMMER = arduino
OBJCOPY = avr-objcopy


all:$(TARGET).hex

# This is  the first dependancy, thus the base file name needs to be passed in (?)
$(TARGET).o: $(TARGET).c $(DEPENDENCIES)
	@ $(CC) $(CFLAGS) -mmcu=$(DEVICE) -DF_CPU=$(FREQ) $(TARGET).c $(DEPENDENCIES) -o $(TARGET).o

# The '%'' sign makes the Make file more generic, can be replaced by $(TARGET)
# $< is the dependancy and $@ is the target file


# Since the the rules are chained together Make deletes intermediate files
# automatically. Use ".PRECIOUS: %.elf" to let Make know to keep the intermediate
# files

%.hex: %.o
	@ $(OBJCOPY) -j .text -j .data -O ihex $< $@
	@ echo "Success!!"

# Use avrdude to flash the chip
# Syntax : avrdude -c <programmer>
# 				-p <part #> -P <port>
#					-U flash\eeprom:w\r\v:<input file>
# use -b <baud rate> to over-ride baudrate
# http://www.ladyada.net/learn/avr/avrdude.html

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(DEVICE) -P $(PORT) -U flash:w:$<


clean:
	@ rm *.o *.hex


