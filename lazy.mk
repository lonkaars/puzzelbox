# this file is for lazy people (loek)

BUILD_DIR ?= build
TARGET ?= $(BUILD_DIR)/main

.PHONY: FORCE

all: FORCE $(TARGET)

$(BUILD_DIR)/build.ninja: CMakeLists.txt
	mkdir -p $(BUILD_DIR)
	cmake -B $(BUILD_DIR) -G Ninja --fresh --log-level WARNING

$(TARGET): $(BUILD_DIR)/build.ninja FORCE
	ninja -C $(BUILD_DIR)
# ninja automatically builds in parallel, so is preferred

clean: FORCE
	$(RM) -r $(BUILD_DIR)

