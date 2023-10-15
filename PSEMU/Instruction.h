#pragma once
#include <cstdint>

struct Instruction {
    uint32_t instruction;

    inline uint32_t function() const {
        return instruction >> 26;
    }

    inline uint32_t t() const {
        return (instruction >> 16) & 0x1f;
    }

    inline uint32_t imm() const {
        return instruction & 0xffff;
    }
};