#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Bios.h"
#include "Range.h"
#include "RAM.h"
#include "DMA.h"

class Bus {
public:
    Bus() { ram.newl(); } // Initialize BIOSRange in the constructor

    // Memory Ranges
    const Range BIOS = Range(0x1fc00000, 512 * 1024);
    const Range RAM_SIZE = Range(0x1f801060, 4);
    const Range CACHE_CONTROL = Range(0xfffe0130, 4);
    const Range RAM_ = Range(0x00000000, 2 * 1024 * 1024);
    const Range SPU = Range(0x1f801c00, 640);
    const Range EXPANSION_2 = Range(0x1f802000, 66);
    const Range EXPANSION_1 = Range(0x1f000000, 512 * 1024);
    const Range IRQ_CONTROL = Range(0x1f801070, 8);
    const Range TIMERS = Range(0x1f801100, 0x30);
    const Range _DMA = Range(0x1f801080, 0x80);
    const Range GPU = Range(0x1f801810, 8);
    const Range MEM_CONTROL = Range(0x1f801000, 36);
    
    uint32_t mask_region(uint32_t addr);

    // Load and Store functions
    uint32_t load32(uint32_t addr);

    void store32(uint32_t addr, uint32_t value);
    
    void store16(uint32_t addr, uint16_t value);

    void store8(uint32_t addr, uint8_t value);

    uint8_t load8(uint32_t addr);
    
    uint16_t load16(uint8_t addr);

    uint32_t dma_reg(uint32_t offset);

    void set_dma_reg(uint32_t offset, uint32_t value);

    Bios bios;
    RAM ram;
    DMA dma;

    const uint32_t region_mask[8] = {
        0xffffffff, 0xffffffff,
        0xffffffff, 0xffffffff,
        0x7fffffff, 0x1fffffff,
        0xffffffff, 0xffffffff,
    };
};
