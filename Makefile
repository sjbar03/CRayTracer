# ===== Compiler =====
CC := gcc

# ===== Flags =====
CFLAGS := -Wall -Wextra -std=c11 -O2
LDFLAGS := -lgdi32 -luser32 -lpthread

# ===== Files =====
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

TARGET := main.exe

# ===== Rules =====
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all run clean
