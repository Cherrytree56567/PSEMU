/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include "Memory.h"

uint8_t Memory::readByte(uint32_t address) const {
    if (address < memory.size()) {
        return memory[address];
    }
    else {
        Logging console;
        console.warn("Out of bounds memory access");
        return 0;
    }
}

void Memory::writeByte(uint32_t address, uint8_t value) {
    if (address < memory.size()) {
        memory[address] = value;
    }
    else {
        Logging console;
        console.warn("Out of bounds memory access");
    }
}

uint32_t Memory::readWord(uint32_t address) const {
    if (address < memory.size() - 3) {
        uint32_t value = 0;
        for (int i = 0; i < 4; ++i) {
            value |= static_cast<uint32_t>(memory[address + i]) << (8 * i);
        }
        return value;
    }
    else {
        Logging console;
        console.warn("Out of bounds memory access");
        return 0;
    }
}

void Memory::writeWord(uint32_t address, uint32_t value) {
    if (address < memory.size() - 3) {
        for (int i = 0; i < 4; ++i) {
            memory[address + i] = static_cast<uint8_t>(value >> (8 * i));
        }
    }
    else {
        Logging console;
        console.warn("Out of bounds memory access");
    }
}

void Memory::writeHalfword(uint32_t address, uint16_t value) {
    if (address % 2 != 0) {
        std::cerr << "Error: Address must be halfword-aligned." << std::endl;
        return;
    }

    if (address >= memory.size()) {
        std::cerr << "Error: Address out of range." << std::endl;
        return;
    }

    memory[address] = static_cast<uint8_t>(value & 0xFF); // Write the least significant byte
    memory[address + 1] = static_cast<uint8_t>((value >> 8) & 0xFF); // Write the most significant byte
}