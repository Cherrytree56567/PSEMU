CC = gcc
CFLAGS = -Wall -Wextra -std=c++2a

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = PSEMU.elf

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)