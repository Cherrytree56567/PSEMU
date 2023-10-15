#include "RAM.h"

void RAM::newl() {
	for (int i = 0; i < (2 * 1024 * 1024); i++) {
		ram[i] = 0xca;
	}
}

uint32_t RAM::load32(uint32_t offset) {
    uint32_t b0 = ram[offset + 0];
    uint32_t b1 = ram[offset + 1];
    uint32_t b2 = ram[offset + 2];
    uint32_t b3 = ram[offset + 3];

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

/// Store the 32bit little endian word `val` into `offset`
void RAM::store32(uint32_t offset, uint32_t value) {
    uint32_t b0 = value;
    uint32_t b1 = (value >> 8);
    uint32_t b2 = (value >> 16);
    uint32_t b3 = (value >> 24);

    ram[offset + 0] = b0;
    ram[offset + 1] = b1;
    ram[offset + 2] = b2;
    ram[offset + 3] = b3;
}