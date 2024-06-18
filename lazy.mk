# NOTE: CMAKE IS THE PRIMARY BUILD SYSTEM FOR SUBFOLDERS/LIBRARIES IN THIS
# REPOSITORY. THIS FILE IS PROVIDED PURELY FOR CONVENIENCE, AND SHOULD NOT
# BECOME AN ESSENTIAL PART OF THE BUILD SYSTEM!

BUILD_DIR ?= build
TARGET ?= $(BUILD_DIR)/main

CMFLAGS += --fresh
CMFLAGS += --log-level WARNING
CMFLAGS += -Wno-deprecated

.PHONY: FORCE

all: FORCE $(TARGET)

$(BUILD_DIR)/build.ninja: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) -G Ninja $(CMFLAGS)

$(TARGET): $(BUILD_DIR)/build.ninja FORCE
	@ninja -C $(BUILD_DIR)

clean: FORCE
	$(RM) -r $(BUILD_DIR)

# Forward any unknown targets to Ninja
ifneq ($(MAKECMDGOALS),)
%::
	@ninja -C $(BUILD_DIR) $@
endif

