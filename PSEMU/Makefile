CC = g++-10 
CFLAGS = -std=c++20

SRCS = $(wildcard *.cpp)
EXECUTABLE = PSEMU.elf

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -f $(EXECUTABLE)
