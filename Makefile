ARDUINO_DIR = /home/ildus/Arduino/arduino-1.6.12

BOARD_TAG    = mega
BOARD_SUB    = atmega2560
ARDUINO_LIBS = Wire LiquidCrystal_I2C1602V1 TEA5767
ARDUINO_SKETCHBOOK = ${ARDUINO_DIR}

include /usr/share/arduino/Arduino.mk

control:
	python control.py
