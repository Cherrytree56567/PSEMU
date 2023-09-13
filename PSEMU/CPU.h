/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#pragma once
#include <fstream>
#include <bitset>
#include "Memory.h"
#include "CPURegisters.h"

class CPU {
public:
    CPU(size_t memorySize) : memory(memorySize), numInstructions(0) {}

    void op_add(uint32_t instruction);
    void op_addu(uint32_t instruction);
    void op_storebyte(uint32_t instruction);
    void op_lui(uint32_t instruction);
    void op_addi(uint32_t instruction);

    void loadInstructions();
    void loadBiosCode(uint32_t* binaryCode);

    void run();

    void loadBIOS(const char* filename);

private:
    CPURegisters registers;
    Memory memory;
    size_t numInstructions;
    uint32_t* BiosCode;
    bool checkForInterrupts() {
        // Return true if an interrupt is pending, otherwise return false
        return false;
    }

    void handleInterrupts() {
        // Update the program counter and other relevant registers based on the interrupt
        // You might also need to save the current state and jump to the interrupt handler
    }
};

