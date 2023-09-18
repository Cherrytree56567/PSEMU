#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <climits>
#include <stddef.h>

class GPU {
public:
	// [For GPU Memory]
	GPU(size_t gpmemsize) : GPUmemory(gpmemsize) {}

	std::vector<uint8_t> GPUmemory;
};
