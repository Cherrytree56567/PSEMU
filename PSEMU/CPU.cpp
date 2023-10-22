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