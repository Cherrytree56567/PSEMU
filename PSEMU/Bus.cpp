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
    return;
}