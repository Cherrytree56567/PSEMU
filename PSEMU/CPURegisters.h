/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <cstdint>
#include <vector>
#include "Coprocessor.h"

class CPURegisters {
public:
    CPURegisters(int s) : pc(s), sp(0), lo(0), hi(0) {
        for (int i = 0; i < 32; ++i) {
            reg[i] = 0;
        }
    }
    uint32_t pc = 0;   // Program Counter
    uint32_t next_pc = 0;
    uint32_t sp = 0;   // Stack Pointer
    uint32_t reg[32];  // Array to hold all registers (including zero, at, v0-v1, a0-a3, t0-t9, s0-s7, k0-k1, gp, s8/fp, ra)
    uint32_t lo = 0;   // LO register
    uint32_t hi = 0;   // HI register
    uint i_stat, i_mask;
};
