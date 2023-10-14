#pragma once
#include "Bus.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {
		pc = 0xbfc00000;
	}

	void tick();
	void fetch();
	void decode_execute(uint32_t instruction);

	Bus* bus;
	
	// Registers
	uint32_t pc = 0;
};

