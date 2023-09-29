/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <fstream>
#include <bitset>
#include <type_traits>
#include "Memory.h"
#include "CPURegisters.h"
#include "Coprocessor.h"

class CPU {
public:
    CPU(Memory& memorya) : memory(memorya), numInstructions(0) {}

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
    void op_lhu(uint32_t instruction);
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
    void loadBiosCode(uint32_t* binaryCode, size_t numI);

    void run();

    void loadBIOS(const char* filename);
private:
    CPURegisters registers;
    size_t numInstructions;
    uint32_t* BiosCode;
    Coprocessor0 coprocessor0;
    Memory& memory;
    bool checkForInterrupts() {
        // Check if there is an interrupt request
        uint32_t status = registers.getC0Register(coprocessor0.STATUS);
        bool interruptRequest = (status & 0x0000ff00) != 0;

        return interruptRequest;
    }

    void handleInterrupts() {
        // Check if there is an interrupt request
        bool interruptRequest = checkForInterrupts();
        if (interruptRequest) {
            // Save the current state and jump to the interrupt handler
            uint32_t epc = registers.pc;
            uint32_t status = registers.getC0Register(coprocessor0.STATUS);
            uint32_t cause = registers.getC0Register(coprocessor0.CAUSE);
            uint32_t bad_address = registers.getC0Register(coprocessor0.BAD_ADDRESS);
            uint32_t handler = memory.read32(0x80000180); // Interrupt handler address
            registers.setC0Register(coprocessor0.STATUS, status | 0x00100000); // Set the exception level
            registers.setC0Register(coprocessor0.CAUSE, cause | 0x80000000); // Set the exception code
            registers.setC0Register(coprocessor0.EPC, epc); // Save the old program counter
            registers.pc = handler; // Jump to the handler
        }
    }
};

