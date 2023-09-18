#pragma once
#include <cstdint>
#include <vector>

class GPU {
public:
	// [For GPU Memory]
	GPU(size_t gpmemsize) : GPUmemory(gpmemsize) {}

	std::vector<uint8_t> GPUmemory;
};
