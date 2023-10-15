#pragma once
#include "Bus.h"
#include "Instruction.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {
		pc = 0xbfc00000;
	}

	// CPU Functions
	void tick();
	void fetch();
	void decode_execute(uint32_t instruction);

	// CPU OPCODES
	void op_lui(uint32_t instruction);

	Bus* bus;
	uint32_t pc;
};

