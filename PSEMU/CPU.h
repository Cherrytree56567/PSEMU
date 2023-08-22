#pragma once
#include "Memory.h"
#include "CPURegisters.h"

class CPU {
public:
    CPU(size_t memorySize) : memory(memorySize) {
        // Initialize any other settings or variables
    }

    void run() {
        while (true) {
            // Fetch opcode from memory at PC
            uint32_t instruction = memory.readWord(registers.pc);
            uint8_t opcode = extractField(instruction, 0, 5);
            uint8_t funct = extractField(instruction, 26, 31);

            switch (opcode) {
                case 0b000000: // R-type instructions
                    switch (funct) {
                        case 0b100000:
                            // Add
                            break;

                        case 0b100001:
                            // addu
                            break;

                        default:
                            // Handle invalid funct code
                            break;
                    }
                    break;

                case 0b001000:
                    // Addi
                    break;

                case 0b001001:
                    // Addiu
                    break;

                case 0b001100:
                    // Andi
                    break;

                // Add more cases for other I-type instructions...

                case 0b000100:
                    // beq
                    break;

                case 0b000110:
                    // blez
                    break;

                case 0b000101:
                    // bne
                    break;

                // Add more cases for other I-type instructions...

                case 0b000010:
                    // j
                    break;

                case 0b000011:
                    // jal
                    break;

                // Add more cases for other J-type instructions...

                case 0b100000: // lb
                    // lb
                    break;

                case 0b100100: // lbu
                    // lbu
                    break;

                default:
                    // Handle invalid opcode
                    break;
            }


            // Program Counter
            registers.pc += 4; // 4 Bytes = 32 Bits

            // Interrupts
            if (checkForInterrupts()) {
                handleInterrupts();
            }
        }
    }

private:
    CPURegisters registers;
    Memory memory;
    uint8_t extractField(uint32_t instruction, int start, int end) {
        uint32_t mask = ((1 << (end - start + 1)) - 1) << start;
        return (instruction & mask) >> start;
    }
    bool checkForInterrupts() {
        // Return true if an interrupt is pending, otherwise return false
        return false;
    }

    void handleInterrupts() {
        // Update the program counter and other relevant registers based on the interrupt
        // You might also need to save the current state and jump to the interrupt handler
    }
};

