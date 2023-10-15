#include "RAM.h"

void RAM::newl() {
	for (int i = 0; i < (2 * 1024 * 1024); i++) {
		ram[i] = 0x0;
	}
}

uint32_t RAM::load32(uint32_t offset) {
    /*uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value |= static_cast<uint32_t>(ram[offset + i]) << (8 * i);
    }
    return value;*/
    return 0;
}

/// Store the 32bit little endian word `val` into `offset`
void RAM::store32(uint32_t offset, uint32_t value) {
    /*for (int i = 0; i < 4; ++i) {
        ram[offset + i] = static_cast<uint8_t>(value >> (8 * i));
    }*/
}