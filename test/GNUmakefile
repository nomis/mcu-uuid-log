.PHONY: all build native doxygen

all: build native doxygen
	python3 version_check.py

build:
	rm -rf build/*/.pio
	# Workaround for https://github.com/platformio/platformio-core/issues/2573
	cd build/example-Serial && pio lib install uuid-common
	platformio run -d build/example-Serial
	build/example-Serial/.pio/build/native/program >../examples/Serial.txt

native:
	rm -rf native/.pio
	platformio test -d native

doxygen:
	wget https://raw.githubusercontent.com/nomis/mcu-uuid-doxygen/master/Doxyfile -O Doxyfile
	rm -rf html
	doxygen
