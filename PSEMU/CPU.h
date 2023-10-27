#pragma once
#include "Bus.h"
#include <tuple>
#include <cstring>
#include "Instruction.h"

enum Exception {
    SysCall = 0x8,
    Overflow = 0xc,
    LoadAddressError = 0x4,
    StoreAddressError = 0x5,
    Break = 0x9,
    CoprocessorError = 0xb,
	IllegalInstruction = 0xa
};

class CPU {
public:
	CPU(Bus* bu) : bus(bu) {}

	// CPU FUNCTIONS
	void tick();
	void fetch();
	void decode_execute(Instruction instruction);

	// CPU OPCODES
	void op_lui();
	void op_ori();
	void op_sw();
	void op_sll();
	void op_addiu();
	void op_j();
	void op_or();
	void op_cop0();
	void op_mtc0();
	void op_bne();
	void op_addi();
	void op_lw();
	void op_sltu();
	void op_addu();
	void op_sh();
	void branch();

	void op_jal();
	void op_andi();
	void op_sb();
	void op_jr();
	void op_lb();
	void op_beq();
	void op_mfc0();
	void op_and();
	void op_add();
	void op_bgtz();
	
	void op_blez();
	void op_lbu();
	void op_jalr();
	void op_bxx();
	void op_slti();
	void op_subu();
	void op_sra();
	void op_div();
	void op_mflo();
	void op_srl();
	void op_sltiu();
	void op_divu();
	void op_mfhi();
	void op_slt();

	void exception(Exception cause);
	void op_syscall();
	void op_mtlo();
	void op_mthi();
	void op_rfe();
	void op_lhu();
	void op_sllv();
	void op_lh();
	void op_nor();
	void op_srav();

	void op_srlv();
	void op_multu();
	void op_xor();
	void op_break();
	void op_mult();
	void op_sub();
	void op_xori();
	void op_cop1();
	void op_cop3();
	void reset();

	void op_cop2();
	void op_lwl();
	void op_lwr();
	void op_swl();
	void op_swr();
	void op_lwc0();
	void op_lwc1();
	void op_lwc2();
	void op_lwc3();
	void op_swc0();
	void op_swc1();
	void op_swc2();
	void op_swc3();

	// HELPER FUNCTIONS
	void set_reg(uint32_t index, uint32_t value) {
		out_regs[index] = value;

		out_regs[0] = 0;
	}

	void store32(uint32_t addr, uint32_t value) {
		if (sr & 0x10000 != 0) {
			// Cache is isolated, ignore write
			std::cout << "[CPU] INFO: Ignore load while cache is isolated\n";
			return;
		}
		bus->store32(addr, value);
	}

	void store16(uint32_t addr, uint32_t value) {
		if (sr & 0x10000 != 0) {
			// Cache is isolated, ignore write
			std::cout << "[CPU] INFO: Ignore load while cache is isolated\n";
			return;
		}
		bus->store16(addr, value);
	}

	void store8(uint32_t addr, uint32_t value) {
		if (sr & 0x10000 != 0) {
			// Cache is isolated, ignore write
			std::cout << "[CPU] INFO: Ignore load while cache is isolated\n";
			return;
		}
		bus->store8(addr, value);
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
	uint32_t sr; // COP0 Status Register
	uint32_t out_regs[32];
	std::tuple<uint32_t, uint32_t> load;
	bool delay_slot, brancha;
	Instruction instr;
};

