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
    const Range RAM_SIZE = Range(0x1f801060, 4);
    const Range CACHE_CONTROL = Range(0xfffe0130, 4);

    // Load and Store functions
    uint32_t load32(uint32_t addr) {
        if (BIOS.contains(addr)) {
            return bios.load32(BIOS.offset(addr));
        } else if (RAM_SIZE.contains(addr)) {
            return 0x00000888;
        }

        if (addr % 4 != 0) {
            throw std::runtime_error("[Bus] ERROR: Unaligned load32 address " + std::to_string(addr));
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled fetch32 at address " + std::to_string(addr));
    }

    void store32(uint32_t addr, uint32_t value) {
        if (MEM_CONTROL.contains(addr)) {
            return;
        } else if (RAM_SIZE.contains(addr)) {
            return;
        } else if (CACHE_CONTROL.contains(addr)) {
            std::cout << "[Bus] WARNING: Cache_Control not implemented.";
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
