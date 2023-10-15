#pragma once
#include "Bus.h"
#include "Instruction.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {
		pc = 0xbfc00000;
		for (int i = 0; i < 32; i++) {
			regs[i] = 0;
		}
	}

	// CPU FUNCTIONS
	void tick();
	void fetch();
	void decode_execute(Instruction instruction);

	// CPU OPCODES
	void op_lui(Instruction instruction);
	void op_ori(Instruction instruction);

	// HELPER FUNCTIONS
	void set_reg(uint32_t index, uint32_t value) {
		regs[index] = value;

		regs[0] = 0;
	}

	Bus* bus;
	uint32_t pc;
	uint32_t regs[32];
};

