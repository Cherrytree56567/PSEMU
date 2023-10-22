#pragma once
#include <cstdint>

struct Instruction {
    uint32_t instruction;

    //  OPCode
    inline uint32_t opcode() const {
        return instruction >> 26;
    }

    inline uint32_t rt() const {
        return (instruction >> 16) & 0x1f;
    }

    inline uint32_t rs() const {
        return (instruction >> 21) & 0x1F;
    }

    inline uint32_t imm() const {
        return instruction & 0xffff;
    }

    inline uint32_t imm_s() const {
        return(uint32_t)(int16_t)imm();
    }

    inline uint32_t rd() const {
        return(instruction >> 11) & 0x1F;
    }

    inline uint32_t sa() const {
        return(instruction >> 6) & 0x1F;
    }

    inline uint32_t function() const {
        return instruction & 0x3F;
    }

    inline uint32_t addr() const {
        return instruction & 0x3FFFFFF;
    }
};