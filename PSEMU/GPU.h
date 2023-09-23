#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <climits>
#include <stddef.h>
#include "Logging.h"

class GPU {
public:
	// [ For GPU Memory ]
	GPU(size_t gpmemsize) : GPUmemory(gpmemsize) {}

    uint8_t& operator[](uint32_t address) {
        if (address < GPUmemory.size()) {
            return GPUmemory[address];
        }
        else {
            Logging Console;
            Console.err(54);
            static uint8_t eval = 0;
            return eval;
        }
    }
private:
	std::vector<uint8_t> GPUmemory;
};
