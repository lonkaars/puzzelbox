TARGET = $(BUILD_DIR)/main.elf

include ../../lazy.mk

export SERIAL_PORT ?= /dev/ttyACM0
flash: upload-main;
upload-main: $(TARGET)

