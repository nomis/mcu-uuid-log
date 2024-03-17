.PHONY: all build native doxygen registry
SHELL=/bin/bash

all: build native doxygen
	python3 version_check.py

build:
	rm -rf build/*/.pio

	# Workaround for https://github.com/platformio/platformio-core/issues/2573
	cd build/example-Serial && pio lib install --no-save nomis/uuid-common
	platformio run -d build/example-Serial
	build/example-Serial/.pio/build/native/program >../examples/Serial.txt

	# Workaround for https://github.com/platformio/platformio-core/issues/2573
	cd build/example-PrintHandler && pio lib install --no-save nomis/uuid-common
	platformio run -d build/example-PrintHandler
	build/example-PrintHandler/.pio/build/native/program >../examples/PrintHandler.txt

native:
	rm -rf native/.pio
	platformio test -d native

doxygen:
	wget https://raw.githubusercontent.com/nomis/mcu-uuid-doxygen/main/Doxyfile -O Doxyfile
	rm -rf html
	(cat Doxyfile; echo INPUT += ../src) | doxygen - 1>doxygen.out 2>doxygen.err
	cat doxygen.err
	cmp /dev/null doxygen.err

registry:
	diff -U0 <(git tag | sort -n) <(platformio pkg show nomis/uuid-log | grep -A 1000000 -E ^Version | tail -n +3 | grep -vE '^$$' | awk '{print $$1}' | sort -n)
