#pragma once
#include <cstdint>
#include <vector>
#include "Logging.h"

class Memory {
public:
    Memory(size_t size) : memory(size) {}

    uint8_t& operator[](uint32_t address) {
        if (address < memory.size()) {
            return memory[address];
        }
        else {
            std::cerr << "Memory access out of bounds: " << address << std::endl;
            // You might want to handle this error situation accordingly.
            // For now, returning a reference to a static variable to indicate an error.
            static uint8_t dummy_error_value = 0;
            return dummy_error_value;
        }
    }

    uint8_t readByte(uint32_t address) const;

    void writeByte(uint32_t address, uint8_t value);

    uint32_t readWord(uint32_t address) const;

    void writeWord(uint32_t address, uint32_t value);

    // TODO: Implement memory-mapped I/O for CD-ROM and HDD
    
    std::vector<uint8_t> memory;
};