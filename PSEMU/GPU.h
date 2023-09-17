#pragma once
#include <cstdint>
#include <vector>

class GPU {
	// [For GPU Memory]
	GPU(size_t memsize) : GPUmemory(memsize) {}

	std::vector<uint8_t> GPUmemory;
};