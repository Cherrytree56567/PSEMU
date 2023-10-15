#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Bios.h"
#include "Range.h"

class Bus {
public:
    Bus() {} // Initialize BIOSRange in the constructor

    // Memory Ranges
    const Range BIOS = Range(0xbfc00000, 512 * 1024);
    const Range MEM_CONTROL = Range(0x1f801000, 36);

    // Load and Store functions
    uint32_t load32(uint32_t addr) {
        auto offset = BIOS.contains(addr);

        if (offset != std::nullopt) {
            return bios.load32(offset.value());
        }

        if (addr % 4 != 0) {
            throw std::runtime_error("[Bus] ERROR: Unaligned load32 address " + std::to_string(addr));
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled fetch32 at address " + std::to_string(addr));
    }

    void store32(uint32_t addr, uint32_t value) {
        auto offset = MEM_CONTROL.contains(addr);
        if (offset != std::nullopt) {
            return;
        }


        if (addr % 4 != 0) {
            throw std::runtime_error("[Bus] ERROR: Unaligned store32 address " + std::to_string(addr));
            return;
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled store32 into address " + std::to_string(addr));
    }

    Bios bios;
};
