CC = g++-13
CFLAGS = -std=c++23 -Wall

SRCS = $(wildcard *.cpp)
EXECUTABLE = PSEMU.elf

# Test runner: runs the CPU on a raw MIPS binary without a BIOS.
TEST_RUNNER = r3000_test.elf
TEST_SRCS = Testing/r3000_test.cpp $(filter-out PSEMU.cpp,$(SRCS))

# Cross binutils, only needed by `make test-bin`: apt install binutils-mipsel-linux-gnu
MIPS_PREFIX = mipsel-linux-gnu-
TEST_DIR = Testing/R3000
TEST_BASE = 0x80010000

# Values the program in $(TEST_DIR)/basic.s must leave behind.
TEST_EXPECT = \
	--expect t2=30 --expect t3=0x7fffffff --expect t4=0x80000009 \
	--expect t5=7 --expect t6=0xffffffff --expect t7=10 \
	--expect s1=30 --expect s2=0x80000009 --expect s3=0xab --expect s6=0xffffffab \
	--expect s4=15 --expect s5=0 \
	--expect v0=1 --expect v1=0 --expect a0=5 --expect a1=7 \
	--expect-mem 0x80020000=30 --expect-mem 0x80020004=0x80000009 --expect-mem 0x80020008=0xcacacaab

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

$(TEST_RUNNER): $(TEST_SRCS) $(wildcard *.h)
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $@

# Runs the test binary, then checks that a wrong expectation is reported as a failure.
test: $(TEST_RUNNER)
	./$(TEST_RUNNER) --load $(TEST_BASE) $(TEST_EXPECT) $(TEST_DIR)/basic.bin
	@echo "Checking that a wrong expected value makes the runner fail"
	@if ./$(TEST_RUNNER) --load $(TEST_BASE) --expect t2=31 $(TEST_DIR)/basic.bin; then \
		echo "ERROR: runner passed with a wrong expectation"; exit 1; \
	else echo "ok: runner failed as it should"; fi

# Rebuilds the committed test binary from its assembly source.
test-bin:
	$(MIPS_PREFIX)as -march=r3000 -o $(TEST_DIR)/basic.o $(TEST_DIR)/basic.s
	$(MIPS_PREFIX)ld -Ttext=$(TEST_BASE) -e start -o $(TEST_DIR)/basic.elf $(TEST_DIR)/basic.o
	$(MIPS_PREFIX)objcopy -O binary -j .text $(TEST_DIR)/basic.elf $(TEST_DIR)/basic.bin
	rm -f $(TEST_DIR)/basic.o $(TEST_DIR)/basic.elf

clean:
	rm -f $(EXECUTABLE) $(TEST_RUNNER)

.PHONY: all test test-bin clean
