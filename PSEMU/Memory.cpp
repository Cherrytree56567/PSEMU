/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include "Memory.h"

uint8_t Memory::readByte(uint32_t address) const {
    if (address < MainRAM.size()) {
        return MainRAM[address];
    }
    else {
        Logging console;
        console.err(54);
        return 0;
    }
}

void Memory::writeByte(uint32_t address, uint8_t value) {
    if (address < MainRAM.size()) {
        MainRAM[address] = value;
    }
    else {
        Logging console;
        console.err(54);
    }
}

uint32_t Memory::readWord(uint32_t address) const {
    if (address < MainRAM.size() - 3) {
        uint32_t value = 0;
        for (int i = 0; i < 4; ++i) {
            value |= static_cast<uint32_t>(MainRAM[address + i]) << (8 * i);
        }
        return value;
    }
    else {
        Logging console;
        console.err(54);
        return 0;
    }
}

void Memory::writeWord(uint32_t address, uint32_t value) {
    if (address < MainRAM.size() - 3) {
        for (int i = 0; i < 4; ++i) {
            MainRAM[address + i] = static_cast<uint8_t>(value >> (8 * i));
        }
    }
    else {
        Logging console;
        console.err(54);
    }
}

void Memory::writeHalfword(uint32_t address, uint16_t value) {
    if (address % 2 != 0) {
        Logging console;
        console.err(55);
        return;
    }

    if (address >= MainRAM.size()) {
        Logging console;
        console.err(56);
        return;
    }

    MainRAM[address] = static_cast<uint8_t>(value & 0xFF); // Write the least significant byte
    MainRAM[address + 1] = static_cast<uint8_t>((value >> 8) & 0xFF); // Write the most significant byte
}

uint32_t Memory::read32(uint32_t address) const {
    const uint32_t value = *(const uint32_t*)&MainRAM[address];
    return value;
}