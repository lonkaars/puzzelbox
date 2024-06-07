TARGET = $(BUILD_DIR)/main.elf

include ../../lazy.mk

export SERIAL_PORT ?= /dev/ttyACM0
flash: upload-main;

test: test_a test_b;

test_a:
	$(MAKE) -C . clean
	$(MAKE) -E CMFLAGS+=-D\ CMAKE_CXX_FLAGS=-DTEST_A -C .
	$(MAKE) -E SERIAL_PORT=/dev/ttyACM0 -C . flash

test_b:
	$(MAKE) -C . clean
	$(MAKE) -E CMFLAGS+=-D\ CMAKE_CXX_FLAGS=-DTEST_B -C .
	$(MAKE) -E SERIAL_PORT=/dev/ttyACM1 -C . flash

