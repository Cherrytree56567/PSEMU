#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <climits>
#include <stddef.h>
#include "Logging.h"

class GPU {
public:
	// [For GPU Memory]
	GPU(size_t gpmemsize) : GPUmemory(gpmemsize) {}

    uint8_t& operator[](uint32_t address) {
        if (address < GPUmemory.size()) {
            return GPUmemory[address];
        }
        else {
            Logging Console;
            Console.err(54);
            // You might want to handle this error situation accordingly.
            // For now, returning a reference to a static variable to indicate an error.
            static uint8_t dummy_error_value = 0;
            return dummy_error_value;
        }
    }
private:
	std::vector<uint8_t> GPUmemory;
};
