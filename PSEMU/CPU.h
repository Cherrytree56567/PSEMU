#pragma once
#include "Memory.h"
#include "CPURegisters.h"

class CPU {
public:
    CPU(size_t memorySize) : memory(memorySize) {
        // Initialize any other settings or variables
    }

    void op_add(uint32_t instruction, CPURegisters& registers, int currentbit) {
        uint8_t rs = extractField(instruction, currentbit+, 25);
        uint8_t rt = extractField(instruction, 16, 20);
        uint8_t rd = extractField(instruction, 11, 15);

        uint32_t rsValue = registers.reg[rs];
        uint32_t rtValue = registers.reg[rt];

        // Perform the add operation
        uint32_t result = rsValue + rtValue;

        // Store the result in the destination register
        registers.reg[rd] = result;

        // Update program counter
        registers.pc += 4;
    }

    void loadInstructions(const char* binaryCode, size_t size) {
        // Copy binary code into memory starting at address 0x0
        memcpy(memory.getMemoryPointer(), binaryCode, size);
    }

    void run() {
        while (true) {
            // Fetch opcode from memory at PC
            int currentbit = 0;
            uint32_t instruction = memory.readWord(registers.pc);
            uint8_t opcode = extractField(instruction, currentbit, currentbit+5);
            uint8_t funct = extractField(instruction, currentbit+31-6, currentbit+31);

            switch (opcode) {
                case 0b000000: // R-type instructions
                    switch (funct) {
                        case 0b100000:
                            // Add
                            break;

                        case 0b100001:
                            // Addu
                            break;

                        case 0b100100:
                            // and
                            break;

                        case 0b011010:
                            // div
                            break;

                        case 0b011011:
                            // divu
                            break;

                        case 0b001001:
                            // jalr
                            break;

                        case 0b001000:
                            // jr
                            break;

                        case 0b010000:
                            // mfhi
                            break;

                        case 0b010001:
                            // mthi
                            break;

                        case 0b010010:
                            // mflo
                            break;

                        case 0b010011:
                            // mtlo
                            break;

                        case 0b011000:
                            // mult
                            break;

                        case 0b011001:
                            // multu
                            break;

                        case 0b100111:
                            // nor
                            break;

                        case 0b100110:
                            // xor
                            break;

                        case 0b100101:
                            // or
                            break;

                        case 0b101010:
                            // slt
                            break;

                        case 0b101011:
                            // sltu
                            break;

                        case 0b000000:
                            // sll
                            break;

                        case 0b000010:
                            // srl
                            break;

                        case 0b000011:
                            // sra
                            break;

                        case 0b100010:
                            // sub
                            break;

                        case 0b100011:
                            // subu
                            break;

                        default:
                            // Handle invalid funct code
                            Logging console;
                            console.warn("Invalid Function code");
                            break;
                    }
                    break;

                // J Type Instructions
                case 0b000010:
                    // j
                    break;

                case 0b000011:
                    // jal
                    break;

                // I Type Instructions
                case 0b001000:
                    // addi
                    break;

                case 0b001001:
                    // addiu
                    break;

                case 0b001100:
                    // andi
                    break;

                case 0b000100:
                    // beq
                    break;

                case 0b000110:
                    // blez
                    break;

                case 0b000101:
                    // bne
                    break;

                case 0b000111:
                    // bgtz
                    break;

                case 0b100000:
                    // lb
                    break;

                case 0b100100:
                    // lbu
                    break;

                case 0b100101:
                    // lhu
                    break;

                case 0b001111:
                    // lui
                    break;

                case 0b100011:
                    // lw
                    break;

                case 0b001101:
                    // ori
                    break;

                case 0b101000:
                    // sb
                    break;

                case 0b101001:
                    // sh
                    break;

                case 0b001010:
                    // slti
                    break;

                case 0b001011:
                    // sltiu
                    break;

                case 0b101011:
                    // sw
                    break;

                default:
                    Logging console;
                    console.warn("Invalid Opcode");
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

