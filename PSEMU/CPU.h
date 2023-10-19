#pragma once
#include "Bus.h"
#include <tuple>
#include "Instruction.h"

enum Exception {
    SysCall = 0x8,
	Overflow = 0xc,
	LoadAddressError = 0x4,
	/// Address error on store
	StoreAddressError = 0x5
};

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {
		pc = 0xbfc00000;
		for (int i = 0; i < 32; i++) {
			regs[i] = 0;
		}
		for (int i = 0; i < 32; i++) {
			out_regs[i] = 0;
		}
		load = std::make_tuple(0,0);
		Next_Instr.instruction = 0xbfc00000;
		just_started = true;
		sr = 0;
		hi = 0;
		lo = 0;
		cause = 0;
		epc = 0;
		next_pc = pc + 4;
		delay_slot = false;
		brancha = false;
	}

	// CPU FUNCTIONS
	void tick();
	void fetch();
	void decode_execute(Instruction instruction);

	// CPU OPCODES
	void op_lui(Instruction instruction);
	void op_ori(Instruction instruction);
	void op_sw(Instruction instruction);
	void op_sll(Instruction instruction);
	void op_addiu(Instruction instruction);
	void op_j(Instruction instruction);
	void op_or(Instruction instruction);
	void op_cop0(Instruction instruction);
	void op_mtc0(Instruction instruction);
	void op_bne(Instruction instruction);
	void op_addi(Instruction instruction);
	void op_lw(Instruction instruction);
	void op_sltu(Instruction instruction);
	void op_addu(Instruction instruction);
	void op_sh(Instruction instruction);
	void branch(uint32_t offset);

	void op_jal(Instruction instruction);
	void op_andi(Instruction instruction);
	void op_sb(Instruction instruction);
	void op_jr(Instruction instruction);
	void op_lb(Instruction instruction);
	void op_beq(Instruction instruction);
	void op_mfc0(Instruction instruction);
	void op_and(Instruction instruction);
	void op_add(Instruction instruction);
	void op_bgtz(Instruction instruction);
	
	void op_blez(Instruction instruction);
	void op_lbu(Instruction instruction);
	void op_jalr(Instruction instruction);
	void op_bxx(Instruction instruction);
	void op_slti(Instruction instruction);
	void op_subu(Instruction instruction);
	void op_sra(Instruction instruction);
	void op_div(Instruction instruction);
	void op_mflo(Instruction instruction);
	void op_srl(Instruction instruction);
	void op_sltiu(Instruction instruction);
	void op_divu(Instruction instruction);
	void op_mfhi(Instruction instruction);
	void op_slt(Instruction instruction);

	void exception(Exception cause);
	void op_syscall(Instruction instruction);
	void op_mtlo(Instruction instruction);
	void op_mthi(Instruction instruction);
	void op_rfe(Instruction instruction);
	void op_lhu(Instruction instruction);
	void op_sllv(Instruction instruction);
	void op_lh(Instruction instruction);
	void op_nor(Instruction instruction);
	void op_srav(Instruction instruction);

	void op_srlv(Instruction instruction);
	void op_multu(Instruction instruction);
	void op_xor(Instruction instruction);

	// HELPER FUNCTIONS
	void set_reg(uint32_t index, uint32_t value) {
		out_regs[index] = value;

		out_regs[0] = 0;
	}

	Bus* bus;
	uint32_t pc;
	uint32_t next_pc;
	uint32_t current_pc;
	uint32_t cause;
	uint32_t epc;
	uint32_t regs[32];
	uint32_t hi;
	uint32_t lo;
	Instruction Next_Instr;
	bool just_started;
	uint32_t sr; // COP0 Status Register
	uint32_t out_regs[32];
	std::tuple<uint32_t, uint32_t> load;
	bool delay_slot, brancha;
};

