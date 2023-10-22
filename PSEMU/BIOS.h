#pragma once
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>

class Bios {
public:
	Bios() : data(512*1024) {}
	
	void newl(const char* path);
	uint32_t load32(uint32_t offset);
	void store32(uint32_t offset, uint32_t store);
	uint8_t load8(uint32_t offset) { return data[offset]; }
	
	const uint64_t bios_size = 512 * 1024;
	std::vector<uint8_t> data;
};

