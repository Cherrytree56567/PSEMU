/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "Bus.h"

uint32_t Bus::load32(uint32_t address) {

    if (address % 4 != 0) {
        std::cout << "[BUS] Error: Unaligned load32 address : " + std::to_string(address) << "\n";
    }

    if (auto offset = _BIOS.contains(address); offset.has_value()) {
        return bios.load32(offset.value());
    }

    std::cout << "[BUS] Error: Unhandled fetch32 at address : " + std::to_string(address) << "\n";
}

void Bus::store32(uint32_t address, uint32_t value) {
    if (address % 4 != 0) {
        std::cout << "[BUS] Error: Unaligned store32 address : " + std::to_string(address) << "\n";
    }

    if (auto offset = MEM_CONTROL.contains(address);offset.has_value()) {
        switch (offset.value()) {
            case 0: // Expansion 1 base address
                if (value != 0x1f000000) {
                    std::cout << "[Bus] ERROR: Bad expansion 1 base address : " << std::to_string(value) << "\n";
                }
                break;
            case 4: // Expansion 2 base address
                if (value != 0x1f802000) {
                    std::cout << "[Bus] ERROR: Bad expansion 2 base address : " << std::to_string(value) << "\n";
                }
                break;
            default:
                std::cout << "[Bus] ERROR: Unhandled write to MEM_CONTROL Register.\n";
                break;
        }
        return;
    }

    std::cout << "[BUS] Error: Unhandled store32 at address : " + std::to_string(address) << "\n";
}