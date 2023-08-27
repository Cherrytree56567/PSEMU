/*
 *************************************
 *           PSEMU Licence           *
 *************************************
 
 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include <iostream>
#include "CPU.h"

int main()
{
    uint32_t biosCode[] = {
        0b10100010100101000000011111010000,
    };
    size_t numInstructions = sizeof(biosCode) / sizeof(uint32_t);

    // Create a CPU instance
    CPU cpu(2048); // Specify the memory size

    // Load BIOS code into the CPU's memory
    //cpu.loadBiosCode(biosCode);
    cpu.loadBIOS("scph1001.bin");
    cpu.loadInstructions();

    // Run the CPU to execute the loaded BIOS code
    cpu.run();
}
