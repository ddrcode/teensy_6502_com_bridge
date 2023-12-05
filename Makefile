FQBN ?= teensy:avr:teensy41
PORT ?= usb:8443000

build:
	arduino-cli compile -b $(FQBN) .

upload:
	arduino-cli upload -b $(FQBN) --port $(PORT) .


