BOARD_MAP=matrix.map

ARDUINO_LIBS = TimerOne
ARDUINO_PORT = /dev/ttyACM0

ifeq (1, 1)
BOARD_TAG = leonardo

#ARDUINO_PORT = usb
#BOARD_TAG=uno
#AVRDUDE_ARD_PROGRAMMER=stk500v2
#BOARDS_TXT=$(ARDUINO_SKETCHBOOK)/hardware/formulad/boards.txt
#RESET_CMD = /bin/true
else

BOARD_TAG = minimus32
BOARDS_TXT=$(ARDUINO_SKETCHBOOK)/hardware/minimus/boards.txt
ARDUINO_VAR_PATH=$(ARDUINO_SKETCHBOOK)/hardware/minimus/variants
ARDUINO_CORE_PATH=$(ARDUINO_SKETCHBOOK)/hardware/minimus/cores/minimus
endif

include /home/paul/bin/Arduino.mk

board.h: gen-board-map.py $(BOARD_MAP)
	./gen-board-map.py < $(BOARD_MAP) > board.h

