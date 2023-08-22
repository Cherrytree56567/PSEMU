#pragma once
#include <cstdint>
#include <vector>
#include "Logging.h"

class Memory {
public:
    Memory(size_t size) : memory(size) {}

    uint8_t readByte(uint32_t address) const;

    void writeByte(uint32_t address, uint8_t value);

    uint32_t readWord(uint32_t address) const;

    void writeWord(uint32_t address, uint32_t value);

    // TODO: Implement memory-mapped I/O for CD-ROM and HDD

private:
    std::vector<uint8_t> memory;
};