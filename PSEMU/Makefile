CC = g++-13
CFLAGS = -std=c++23 -Wall

SRCS = $(wildcard *.cpp)
EXECUTABLE = PSEMU.elf

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -f $(EXECUTABLE)
