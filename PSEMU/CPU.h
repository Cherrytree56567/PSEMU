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
#include "GTE.h"
#include "DMA.h"

class DMA;

class CPU {
public:
    CPU(Memory& memorya) : memory(memorya), numInstructions(0) {}

    void op_add(uint32_t instruction);
    void op_addu(uint32_t instruction);
    void op_storebyte(uint32_t instruction);
    void op_lui(uint32_t instruction);
    void op_addi(uint32_t instruction);
    void op_cop0(uint32_t instruction);
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
    void op_rfe(uint32_t instruction);
    void op_mtc0(uint32_t instruction);
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
    void op_lh(uint32_t instruction);
    void op_break(uint32_t instruction);
    void op_srlv(uint32_t instruction);
    void op_srav(uint32_t instruction);
    void op_sllv(uint32_t instruction);
    void op_lwl(uint32_t instruction);
    void op_lwr(uint32_t instruction);
    void op_bcond(uint32_t instruction);
    void op_xori(uint32_t instruction);
    void op_swl(uint32_t instruction);
    void op_swr(uint32_t instruction);
    void op_swc2(uint32_t instruction);
    void op_lwc2(uint32_t instruction);
    void op_mfc2(uint32_t instruction);
    void op_mtc2(uint32_t instruction);
    void op_cfc2(uint32_t instruction);
    void op_ctc2(uint32_t instruction);
    void op_cop2(uint32_t instruction);

    void loadInstructions();
    void loadBiosCode(uint32_t* binaryCode, size_t numI);

    void run();

    void loadBIOS(const char* filename);

    Logging console;
    size_t numInstructions;
    uint32_t* BiosCode;
    Cop0 cop0;
    GTE cop2;
    CPURegisters registers;
    Memory& memory;
    bool is_branch, is_delay_slot;
    bool took_branch;
    bool in_delay_slot_took_branch;
    uint exception_addr[2] = { 0x80000080, 0xBFC00180 };

    void handleInterrupts() {
        uint32_t instr = memory.readWord(registers.pc) >> 26;
        
        if (instr == 0x12) {
          return;
        }
        
        bool pending = (registers.i_stat & registers.i_mask) != 0;
        if (pending) cop0.cause.IP |= (1 << 0);
		    else cop0.cause.IP &= ~(1 << 0);
        
        bool irq_enabled = cop0.sr.IEc;
        uint irq_mask = (cop0.sr.raw >> 8) & 0xFF;
        uint irq_pending = (cop0.cause.raw >> 8) & 0xFF;
        
        if (irq_enabled && (irq_mask & irq_pending) > 0) {
          uint mode = cop0.sr.raw & 0x3F;
          cop0.sr.raw &= ~(uint)0x3F;
          cop0.sr.raw |= (mode << 2) & 0x3F;

          uint copy = cop0.cause.raw & 0xff00;
          cop0.cause.exc_code = (uint)0x0;
          cop0.cause.CE = 1;

          cop0.epc = registers.pc;

        /* Hack: related to the delay of the ex interrupt*/
          is_delay_slot = is_branch;
          in_delay_slot_took_branch = took_branch;

          if (is_delay_slot) {
             cop0.epc -= 4;

             cop0.cause.BD = true;
             cop0.TAR = registers.pc;

             if (in_delay_slot_took_branch) {
                 cop0.cause.BT = true;
             }
          }

    /* Select exception address. */
          registers.pc = exception_addr[cop0.sr.BEV];
          registers.next_pc = registers.pc + 4;
        }
    }
};

