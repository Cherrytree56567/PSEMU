/*
 *************************************
 *           PSEMU Licence           *
 *************************************
 
 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include <iostream>
#include "CPURegisters.h"
#include "Memory.h"

int main()
{
    CPURegisters registers;
    Memory memory(2048);
    std::cout << "PSEMU v1.0\n";

    /* 
    Example usage
        registers.pc = 0x100;
        uint8_t opcode = memory.readByte(registers.pc);
        uint32_t wordValue = memory.readWord(registers.pc);
    */
}