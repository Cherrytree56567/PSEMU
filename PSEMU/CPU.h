/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#pragma once
#include <fstream>
#include <bitset>
#include "Memory.h"
#include "CPURegisters.h"
#include "Coprocessor.h"

class CPU {
public:
    CPU(size_t memorySize) : memory(memorySize), numInstructions(0) {}

    void op_add(uint32_t instruction);
    void op_addu(uint32_t instruction);
    void op_storebyte(uint32_t instruction);
    void op_lui(uint32_t instruction);
    void op_addi(uint32_t instruction);
    void op_addiu(uint32_t instruction);
    void op_and(uint32_t instruction);
    void op_andi(uint32_t instruction);
    void op_beq(uint32_t instruction);
    void op_blez(uint32_t instruction);
    void op_bne(uint32_t instruction);
    void op_bgtz(uint32_t instruction);
    void op_div(uint32_t instruction);
    void op_divu(uint32_t instruction);
    void op_j(uint32_t instruction);
    void op_jal(uint32_t instruction);
    void op_jalr(uint32_t instruction);
    void op_jr(uint32_t instruction);
    void op_lb(uint32_t instruction);
    void op_lbu(uint32_t instruction);
    void op_lh(uint32_t instruction);
    void op_lhu(uint32_t instruction);
    void op_lui(uint32_t instruction);
    void op_lw(uint32_t instruction);
    void op_mfhi(uint32_t instruction);
    void op_mthi(uint32_t instruction);
    void op_mflo(uint32_t instruction);
    void op_mtlo(uint32_t instruction);
    void op_mfc0(uint32_t instruction);
    void op_mult(uint32_t instruction);
    void op_multu(uint32_t instruction);
    void op_nor(uint32_t instruction);
    void op_xor(uint32_t instruction);
    void op_or(uint32_t instruction);
    void op_ori(uint32_t instruction);
    void op_sb(uint32_t instruction);
    void op_sh(uint32_t instruction);
    void op_slti(uint32_t instruction);
    void op_sltiu(uint32_t instruction);
    void op_slt(uint32_t instruction);
    void op_sltu(uint32_t instruction);
    void op_sll(uint32_t instruction);
    void op_srl(uint32_t instruction);
    void op_sra(uint32_t instruction);
    void op_sub(uint32_t instruction);
    void op_subu(uint32_t instruction);
    void op_sw(uint32_t instruction);

    void loadInstructions();
    void loadBiosCode(uint32_t* binaryCode);

    void run();

    void loadBIOS(const char* filename);

private:
    CPURegisters registers;
    Memory memory;
    size_t numInstructions;
    uint32_t* BiosCode;
    Coprocessor0 coprocessor0;
    bool checkForInterrupts() {
        // Return true if an interrupt is pending, otherwise return false
        return false;
    }

    void handleInterrupts() {
        // Update the program counter and other relevant registers based on the interrupt
        // You might also need to save the current state and jump to the interrupt handler
    }
};

