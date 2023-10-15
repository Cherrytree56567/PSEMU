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

}
