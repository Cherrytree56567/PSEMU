#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Bios.h"
#include "Range.h"
#include "RAM.h"

class Bus {
public:
    Bus() { ram.newl(); } // Initialize BIOSRange in the constructor

    // Memory Ranges
    const Range BIOS = Range(0x1fc00000, 512 * 1024);
    const Range SYS_CONTROL = Range(0x1f801000, 36);
    const Range RAM_SIZE = Range(0x1f801060, 4);
    const Range CACHE_CONTROL = Range(0xfffe0130, 4);
    const Range RAM_ = Range(0x00000000, 2 * 1024 * 1024);
    
    uint32_t mask_region(uint32_t addr) {
      // Index address space in 512MB chunks
      uint32_t index = (addr >> 29);

      return addr & region_mask[index];
    }

    // Load and Store functions
    uint32_t load32(uint32_t addr) {
        if (BIOS.contains(mask_region(addr))) {
            return bios.load32(BIOS.offset(mask_region(addr)));
        } else if (RAM_SIZE.contains(mask_region(addr))) {
            return 0x00000888;
        } else if (RAM_.contains(mask_region(addr))) {
            return ram.load32(RAM_.offset(mask_region(addr)));
        }

        if (addr % 4 != 0) {
            throw std::runtime_error("[Bus] ERROR: Unaligned load32 address " + std::to_string(addr));
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled fetch32 at address " + std::to_string(addr));
    }

    void store32(uint32_t addr, uint32_t value) {
        if (SYS_CONTROL.contains(mask_region(addr))) {
            return;
        } else if (RAM_SIZE.contains(mask_region(addr))) {
            return;
        } else if (CACHE_CONTROL.contains(addr)) {
            std::cout << "[Bus] WARNING: Cache_Control not implemented.";
            return;
        } else if (RAM_.contains(mask_region(addr))) {
            ram.store32(RAM_.offset(mask_region(addr), value);
            return;
        }

        if (addr % 4 != 0) {
            throw std::runtime_error("[Bus] ERROR: Unaligned store32 address " + std::to_string(addr));
            return;
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled store32 into address " + std::to_string(addr));
    }

    Bios bios;
    RAM ram;

    const uint32_t region_mask[8] = {
        0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff,
        0x7fffffff, 0x1fffffff,
        0xffffffff, 0xffffffff,
    };
};
