/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include <iostream>
#include "CPU.h"
#include "Memory.h"
#include "DMA.h"
#include "Logging.h"

int main()
{
    uint32_t biosCode[] = {
        0b00111100000000010000000000000001,
        0b00000000001000010000100000100100,
    };
    size_t numInstructions = sizeof(biosCode) / sizeof(uint32_t);

    Memory memory(2048); // Specify the memory size in KB
    CPU cpu(memory);

    // Load BIOS code into the CPU's memory
    //cpu.loadBiosCode(biosCode, numInstructions);
    cpu.loadBIOS("scph1001.bin");
    cpu.loadInstructions();

    // Run the CPU to execute the loaded BIOS code
    cpu.run();
}
