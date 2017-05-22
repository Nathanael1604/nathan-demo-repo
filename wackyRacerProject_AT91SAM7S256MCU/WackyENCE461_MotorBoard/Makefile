RUN_MODE = ROM

OPT = -Os

MMCULIB_DIR = ../../mmculib

DRIVERS = led pacer ir_rc5 bbi2c

PERIPHERALS = pwm adc

TARGET = Motor.bin

SRC = motor.c
SRC += motorLED.c

include ../../boards/sam7hxxx/board.mk
include $(MMCULIB_DIR)/mmculib.mk


