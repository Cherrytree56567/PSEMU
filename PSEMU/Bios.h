#pragma once
#include <vector>

struct Bios_Data {
	/// BIOS memory
	std::vector<uint8_t> data;
};

class Bios {
public:
	Bios() {}
	
	void new(const char* path);
	
	const uint64_t bios_size = 512 * 1024;
	Bios_Data bdata;
};

