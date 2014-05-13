BOARD_MAP=matrix.map

ARDUINO_LIBS = TimerOne charliecube
ARDUINO_PORT = /dev/ttyACM0

BOARD_TAG = leonardo

include ~/bin/Arduino.mk

board.h: gen-board-map.py $(BOARD_MAP)
	./gen-board-map.py < $(BOARD_MAP) > board.h

