.PHONY: all build native

all: build native
	python3 version_check.py

build:
	rm -rf build/*/.pio
	# Workaround for https://github.com/platformio/platformio-core/issues/2573
	cd build/example-Serial && pio lib install uuid-common
	platformio run -d build/example-Serial

native:
	rm -rf native/.pio
	platformio test -d native
