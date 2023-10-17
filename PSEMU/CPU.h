#pragma once
#include "Bus.h"
#include <tuple>
#include "Instruction.h"

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
		Next_Instr.instruction = 0x0;
		just_started = true;
		sr = 0;
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


	// HELPER FUNCTIONS
	void set_reg(uint32_t index, uint32_t value) {
		out_regs[index] = value;

		out_regs[0] = 0;
	}

	Bus* bus;
	uint32_t pc;
	uint32_t regs[32];
	Instruction Next_Instr;
	bool just_started;
	uint32_t sr; // COP0 Status Register
	uint32_t out_regs[32];
	std::tuple<uint32_t, uint32_t> load;
};

