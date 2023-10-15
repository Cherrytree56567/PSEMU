#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Bios.h"
#include "Range.h"

class Bus {
public:
    Bus() {} // Initialize BIOSRange in the constructor

    const Range BIOS = Range(0xbfc00000, 512 * 1024);

    uint32_t load32(uint32_t addr) {
        auto offset = BIOS.contains(addr);
        if (offset != std::nullopt) {
            return bios.load32(offset.value());
        }

        throw std::runtime_error("[Bus] ERROR: Unhandled fetch32 at address " + std::to_string(addr));
    }

    Bios bios;
};
