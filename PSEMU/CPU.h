#pragma once
#include "Bus.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {}

	void tick();
	void fetch();
	void decode_execute();

	Bus* bus;
	
	// Registers
	uint32_t pc = 0;
};

