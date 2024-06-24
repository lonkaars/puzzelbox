TARGET = $(BUILD_DIR)/main.elf

include ../../lazy.mk

export SERIAL_PORT ?= /dev/ttyUSB0
flash: upload;
upload: $(TARGET)

