/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#pragma once
#include <cstdint>
#include <vector>
#include "Logging.h"
#include "GPU.h"

class Memory {
public:
    Memory(size_t size, GPU& gpua) : memory(size), gpu(gpua) {}

    uint8_t& operator[](uint32_t address) {
        if (address < memory.size()) {
            return memory[address];
        } else if (address >= GPU_VRAM_START && address <= GPU_VRAM_END) {
            return gpu[address - GPU_VRAM_START];
        } else {
            Logging console;
            console.err(54);
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

    void writeHalfword(uint32_t address, uint16_t value);

    uint32_t read32(uint32_t address) const;

    // TODO: Implement memory-mapped I/O for CD-ROM and HDD
private:
    GPU& gpu;
    std::vector<uint8_t> memory;

    // Define GPU VRAM address range
    static const uint32_t GPU_VRAM_START = 520093696;  // Start address
    static const uint32_t GPU_VRAM_END = 522190848;    // End address
};
