#include "CPU.h"

void CPU::tick() {
	fetch();
}

void CPU::fetch() {
	instr.value = load32(pc);

	pc += 4;
	decode_execute();
}

void CPU::decode_execute() {
	switch (instr.opcode()) {
	case 0b001111:
		op_lui();
		std::cout << "[CPU] INFO: LUI (I-Type)\n";
		break;

	case 0b001101:
		op_ori();
		std::cout << "[CPU] INFO: ORI (I-Type)\n";
		break;

	case 0b101011:
		op_sw();
		std::cout << "[CPU] INFO: SW (I-Type)\n";
		break;

	default:
		std::cout << "[CPU] ERROR: Unknown Opcode\n";
		exit(0);
	}
}

void CPU::op_lui() {
	auto data = instr.imm();
	auto rt = instr.rt();

	uint32_t val = data << 16;
	set_reg(rt, val);
}

void CPU::op_ori() {
	auto data = instr.imm();
	auto rt = instr.rt();
	auto rs = instr.rs();

	uint32_t val = get_reg(rs) | data;
	set_reg(rt, val);
}

void CPU::op_sw() {
	auto i = instr.imm_s();
	auto rt = instr.rt();
	auto rs = instr.rs();

	uint32_t addr = get_reg(rs) + i;
	uint32_t v = get_reg(rt);

	store32(addr, v);
}