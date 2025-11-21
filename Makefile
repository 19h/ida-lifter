.PHONY: all build clean install

BUILD_DIR := build
PLUGIN_NAME := lifter.dylib
IDA_SDK_PLUGIN_DIR := $(IDASDK)/src/bin/plugins
INSTALL_DIR := $(HOME)/.idapro/plugins

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
	@cd $(BUILD_DIR) && ninja

install: build
	@mkdir -p $(INSTALL_DIR)
	@cp $(IDA_SDK_PLUGIN_DIR)/$(PLUGIN_NAME) $(INSTALL_DIR)/
	@echo "Installed $(PLUGIN_NAME) to $(INSTALL_DIR)"

clean:
	@rm -rf $(BUILD_DIR)
