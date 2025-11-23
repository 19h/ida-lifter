# Detect OS and Architecture
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

CC = gcc
# Base flags
#CFLAGS = -O2 -Wall -Wextra
CFLAGS = -Wall -Wextra

# Architecture specific configuration
ifeq ($(UNAME_S),Darwin)
    ifeq ($(UNAME_M),arm64)
        # Apple Silicon detected: Force x86_64 to trigger Rosetta 2
        # AVX/AVX2 are x86 extensions, not available on ARM64 native.
        CFLAGS += -arch x86_64 -mavx -mavx2
    else
        # Intel Mac
        CFLAGS += -march=native -mavx -mavx2
    endif
else
    # Linux / Windows (assuming x86 environment)
    CFLAGS += -march=native -mavx -mavx2
endif

TARGET = avx_executor
SRCS = main.c avx_ops.c avx2_ops.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
