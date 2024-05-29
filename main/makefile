TARGET = $(BUILD_DIR)/main.uf2

include ../lazy.mk

flash: $(TARGET) FORCE
	picotool load -fx $<
# -f forces a reboot of the pico before flashing
# -x resets the pico after flashing

