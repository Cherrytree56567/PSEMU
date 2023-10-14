#pragma once
#include "Bus.h"

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {}

	Bus* bus;
	void tick();
	void fetch();
	void decode();
	void execute();
};

