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

	std::vector<uint8_t> ram;
};

