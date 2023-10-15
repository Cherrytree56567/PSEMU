#pragma once
#include <cstdint>

class RAM {
public:
	RAM() {}

	void newl();
	uint32_t load32(uint32_t offset);
	void store32(uint32_t offset, uint32_t value);

	uint8_t ram[2 * 1024 * 1024];
};

