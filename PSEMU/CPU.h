#pragma once
#include <string>
#include <cstdint>
#include "Instruction.h"
#include "Bus.h"

class CPU {
public:
	CPU(std::string a) : bus(a) {
		for (int i = 0; i < 32; i++) regs[i] = 0xdeadbeef;
		regs[0] = 0;
		pc = 0xbfc00000;
		instr.value = 0x0;
	}

	void tick();
	void fetch();
	void decode_execute();

	uint32_t load32(uint32_t address) { return bus.load32(address); }
	void store32(uint32_t address, uint32_t value) { bus.store32(address, value); }

	uint32_t reg(uint32_t index) {
		return regs[index];
	}

	void set_reg(uint32_t index, uint32_t val) {
		regs[index] = val;

		// Make sure R0 is always 0
		regs[0] = 0;
	}

	void op_lui();
private:
	uint32_t pc;
	Instruction instr;
	uint32_t regs[32];

	Bus bus;
};

