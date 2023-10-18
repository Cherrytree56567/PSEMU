#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

class RAM {
public:
	RAM() : ram(2 * 1024 * 1024) {}

	void newl();
	uint32_t load32(uint32_t offset);
	void store32(uint32_t offset, uint32_t value);
	void store16(uint32_t offset, uint16_t value);
	void store8(uint32_t offset, uint8_t value) {
		ram[offset] = value;
	}
	uint16_t load16(uint32_t offset);

	/// Fetch the byte at `offset`
	uint8_t load8(uint32_t offset) {
		return ram[offset];
	}

	std::vector<uint8_t> ram;
};

