/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma warning(disable : 4996)
#include "BIOS.h"

BIOS::BIOS(std::string path) {
    FILE* in = fopen(path.c_str(), "rb");
    fread(data, 1, 512 * 1024, in);
    fclose(in);
}

uint32_t BIOS::load32(uint32_t offset) {
    uint32_t b0 = data[offset + 0];
    uint32_t b1 = data[offset + 1];
    uint32_t b2 = data[offset + 2];
    uint32_t b3 = data[offset + 3];

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}