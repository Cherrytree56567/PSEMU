#pragma once
#include <vector>
#include <iostream>
#include "Bios.h"

class Bus {
public:
	Bus(int a) {}

	uint32_t load32(uint32_t addr) {
        if (auto offset = BIOS.contains(addr); offset.has_value()) {
            return bios.load32(offset.value());
        }

        throw std::runtime_error("Unhandled fetch32 at address " + std::to_string(addr));
    }

	// RAM
	static const int RAMSize = (2048 * 8000) / sizeof(uint8_t);
	uint8_t RAM[RAMSize];
	Bios bios;
	
const Range BIOS(0xbfc00000, 512 * 1024);
};

