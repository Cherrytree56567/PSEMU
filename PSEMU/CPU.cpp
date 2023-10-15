#include "CPU.h"

void CPU::tick() {
  fetch();
}

void CPU::fetch() {
    uint32_t instruction = bus->load32(pc);

    // Increment PC to point to the next instruction.
    pc += 4;

    decode_execute(instruction);
}

void CPU::decode_execute(uint32_t instruction) {
    Instruction instr;
    instr.instruction = instruction;
    switch (instr.function()) {
        case (0b001111):
            op_lui(instruction);
            std::cout << "[CPU] INFO: LUI (FUNCTION)\n";
            break;
            
        default:
            std::cout << "[CPU] ERROR: Unhandled Instruction \n";
            exit(0);
            break;
    }
}

void CPU::op_lui(uint32_t instruction) {

}