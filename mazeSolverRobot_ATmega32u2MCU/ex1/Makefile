# File:   Makefile for f_main for ENEL300 Project 2 (Group 6)
# Author: Nathan Park

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: ex1.out


# Compile: create object files from C source files.
ex1.o: ex1.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
ex1.out: ex1.o system.o pio.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: ex1.out
	$(OBJCOPY) -O ihex ex1.out ex1.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash ex1.hex; dfu-programmer atmega32u2 start


