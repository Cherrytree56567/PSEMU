CC = g++
CFLAGS = 

SRCS = $(wildcard *.cpp)
EXECUTABLE = PSEMU.elf

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -f $(EXECUTABLE)