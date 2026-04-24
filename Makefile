.PHONY: all build clean install test test-avx10

BUILD_DIR := build
INSTALL_DIR := $(HOME)/.idapro/plugins

# Detect platform and set plugin extension
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    PLUGIN_EXT := dylib
else
    PLUGIN_EXT := so
endif
PLUGIN_NAME := lifter.$(PLUGIN_EXT)

# Find IDA SDK - check common locations
ifndef IDASDK
    ifneq (,$(wildcard $(HOME)/ida-sdk))
        IDASDK := $(HOME)/ida-sdk
    else ifneq (,$(wildcard $(HOME)/idasdk91))
        IDASDK := $(HOME)/idasdk91
    endif
endif

IDA_SDK_PLUGIN_DIR := $(IDASDK)/src/bin/plugins

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
	@cd $(BUILD_DIR) && ninja

install: build
	@mkdir -p $(INSTALL_DIR)
	@if [ -f "$(IDA_SDK_PLUGIN_DIR)/$(PLUGIN_NAME)" ]; then \
		cp "$(IDA_SDK_PLUGIN_DIR)/$(PLUGIN_NAME)" "$(INSTALL_DIR)/"; \
	else \
		echo "Error: Plugin not found at $(IDA_SDK_PLUGIN_DIR)/$(PLUGIN_NAME)"; \
		exit 1; \
	fi
ifeq ($(UNAME_S),Darwin)
	@codesign -s - -f "$(INSTALL_DIR)/$(PLUGIN_NAME)" 2>/dev/null || true
endif
	@echo "Installed $(PLUGIN_NAME) to $(INSTALL_DIR)"

test:
	@$(MAKE) -C test build

test-avx10:
	@$(MAKE) -C test experimental_avx10

clean:
	@rm -rf $(BUILD_DIR)
