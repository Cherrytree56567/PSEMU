#pragma once
#include <vector>
#include <cstdint>

class Bios {
public:
	Bios() : data(512*1024) {}
	
	void newl(const char* path);
	uint32_t load32(uint32_t offset);
	
	const uint64_t bios_size = 512 * 1024;
	std::vector<uint8_t> data;
};

