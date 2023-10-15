#include "CPU.h"

void CPU::tick() {
  fetch();
}

void CPU::fetch() {
    uint32_t instruction = bus->load32(pc);

    // Increment PC to point to the next instruction.
    pc += 4;

    Instruction instr;
    instr.instruction = instruction;

    decode_execute(instr);
}

void CPU::decode_execute(Instruction instruction) {
    switch (instruction.opcode()) {
        case (0b001111):
            op_lui(instruction);
            std::cout << "[CPU] INFO: LUI (I-Type)\n";
            break;

        case (0b001101):
            op_ori(instruction);
            std::cout << "[CPU] INFO: ORI (I-Type)\n";
            break;

        case (0b101011):
            op_sw(instruction);
            std::cout << "[CPU] INFO: SW (I-Type)\n";
            break;
            
        default:
            std::cout << "[CPU] ERROR: Unhandled Instruction \n";
            exit(0);
            break;
    }
}

void CPU::op_lui(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();

    uint32_t v = i << 16;

    set_reg(t, v);
}

void CPU::op_ori(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] | i;

    set_reg(t, v);
}

void CPU::op_sw(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    bus->store32(addr, v);
}