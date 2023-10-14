#pragma once
#include <vector>
#include <iostream>
#include "Bios.h"

class Bus {
public:
	Bus(int a) {}

	uint32_t read32(uint32_t address) {
		
	}

	// RAM
	static const int RAMSize = (2048 * 8000) / sizeof(uint8_t);
	uint8_t RAM[RAMSize];
	Bios bios;
};

