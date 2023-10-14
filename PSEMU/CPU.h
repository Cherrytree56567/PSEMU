#pragma once
#include "Bus.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {}

	void tick();
	void fetch();
	void decode_execute(uint32_t instruction);

	Bus* bus;
	
	// Registers
	uint32_t pc = 0;
};

