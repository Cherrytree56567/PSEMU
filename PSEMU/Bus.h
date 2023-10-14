#pragma once
#include <vector>
#include <iostream>

class Bus {
public:
	Bus(int a) {}

	uint32_t read32(uint32_t address) {
		if (address < (2048 * 8000) / sizeof(uint8_t)) {
			uint32_t value = 0;
			for (int i = 0; i < 4; ++i) {
				value |= static_cast<uint32_t>(RAM[address + i]) << (8 * i);
			}
			return value;
		}
		else {
			std::cout << "[BUS] Memory Read Address Out of Range.\n";
		}
	}

	// RAM
	static const int RAMSize = (2048 * 8000) / sizeof(uint8_t);
	uint8_t RAM[RAMSize];
};

