/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <cstdint>
#include <vector>
#include "Logging.h"
#include "DMA.h"
#include "CPURegisters.h"

class Memory {
public:
    // size = kilobytes
    DMA *dma;
    Memory(size_t size, CPURegisters &c) : MainRAM((size * 8000) / sizeof(uint8_t)), dma(nullptr) {
      dma = new DMA(&c, this);
    }

    // address = bits
    uint8_t& operator[](uint32_t address) {
        if (address < MainRAMEnd) {
            return MainRAM[address - MainRAMStart];
        } else if (address < DMAEnd) {
            //     --FIX--
            return MainRAM[address - DMAStart];
        } else {
            Logging console;
            console.err(54);
            static uint8_t eval = 0;
            return eval;
        }
    }

    uint8_t readByte(uint32_t address) const;

    void writeByte(uint32_t address, uint8_t value);

    uint32_t readWord(uint32_t address) const;

    void writeWord(uint32_t address, uint32_t value);

    void writeHalfword(uint32_t address, uint16_t value);

    uint32_t read32(uint32_t address) const;

    // TODO: Implement memory-mapped I/O for CD-ROM and HDD
private:
    std::vector<uint8_t> MainRAM;
    int MainRAMStart = 0; // bits
    int MainRAMEnd = MainRAM.size(); // bits
    uint32_t DMAStart = 0x1f801080;
    uint32_t DMAEnd = 0x1f801080 + (uint32_t)0x80LL;
    // Main Ram starts at 0 bits and ends at 16384000 bits (divide it by uint8_t to get array size)
};
