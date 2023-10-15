#pragma once
#include <vector>
#include <iostream>
#include "Bios.h"
#include "Range.h"

class Bus {
public:
    Bus(int a) : BIOSRange(0xbfc00000, 512 * 1024) {} // Initialize BIOSRange in the constructor

    Range BIOSRange; // Declare BIOSRange as a member variable

    uint32_t load32(uint32_t addr) {
        auto offset = BIOSRange.contains(addr);
        if (offset != std::nullopt) {
            return bios.load32(offset.value());
        }

        throw std::runtime_error("Unhandled fetch32 at address " + std::to_string(addr));
    }

    // RAM
    static const int RAMSize = (2048 * 8000) / sizeof(uint8_t);
    uint8_t RAM[RAMSize];
    Bios bios;
};
