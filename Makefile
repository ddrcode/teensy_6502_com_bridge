FQBN ?= teensy:avr:teensy41
PORT ?= usb:8443000

build:
	arduino-cli compile -b $(FQBN) .

debug:
	arduino-cli compile -b $(FQBN) --build-property compiler.cpp.extra_flags=-fDEBUG_TEENSY_COM_BRIDGE -v .

upload:
	arduino-cli upload -b $(FQBN) --port $(PORT) .


