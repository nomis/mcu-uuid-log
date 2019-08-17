.PHONY: all build native

all: build native

build:
	rm -rf build/*/.pio
	platformio run -d build/example-Serial

native:
	rm -rf native/.pio
	platformio test -d native
