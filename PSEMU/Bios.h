#pragma once
#include <vector>

class Bios {
public:
	Bios() {}
	
	void new(const char* path);
	void load32(uint32_t offset);
	
	const uint64_t bios_size = 512 * 1024;
	std::vector<uint8_t>(bios_size) data;
};

