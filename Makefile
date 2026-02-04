# ===== Toolchain =====
CC := gcc

# ===== Project =====
TARGET := ray.exe
BUILD  := build

# Source layout
SRC_DIR := src
INC_DIR := include
THIRD_PARTY := third_party

# cglm include path (header-only)
CGLM_INC := $(THIRD_PARTY)/cglm/include

# ===== Flags =====
CFLAGS  := -Wall -Wextra -std=c11 -O2 -I$(INC_DIR) -I$(CGLM_INC)
LDFLAGS := -lgdi32 -luser32 -lpthread

# If you want to define cglm config macros project-wide, do it here:
# CFLAGS += -DCGLM_ALL_UNALIGNED

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRCS))

# ===== Targets =====
all: $(BUILD) $(TARGET)

$(BUILD):
	@mkdir $(BUILD) 2>NUL || true

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-del /Q $(BUILD)\*.o 2>NUL
	-del /Q $(TARGET) 2>NUL

.PHONY: all run clean
