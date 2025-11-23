UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

CC = clang-20
# Base flags
#CFLAGS = -O2 -Wall -Wextra
CFLAGS = -Wall -Wextra

# Architecture specific configuration
ifeq ($(UNAME_S),Darwin)
    ifeq ($(UNAME_M),arm64)
        # Apple Silicon detected: Force x86_64 to trigger Rosetta 2
        # AVX/AVX2 are x86 extensions, not available on ARM64 native.
        CFLAGS += -arch x86_64 -mavx -mavx2 -mfma
    else
        # Intel Mac
        CFLAGS += -march=native -mavx -mavx2 -mfma
    endif
else
    # Linux / Windows (assuming x86 environment)
    CFLAGS += -march=native -mavx -mavx2 -mfma
endif

TARGET = avx_lifter_test

SRCS = main.c intr_avx.c intr_avx2.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
