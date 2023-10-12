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

/*
 * Somehow get the DMA Working.
 * The DMA needs access to the register and the memory so if put DMA in the CPU and then pass the memory and registers I can allow DMA to work.
 * That was the first part but the second part is to be able to map it.
 *
 * Also, All the CPU Registers have the same value somehow. EDIT: nvm I fixed it, turns out I hadn't initialized all the CPURegister array to 0.
 * Ok, so I have gotton the first part right.
 */

int main() {
    uint32_t biosCode[] = {
        0b00111100000000010000000000000001,
        0b00000000001000010000100000100100,
    };
    size_t numInstructions = sizeof(biosCode) / sizeof(uint32_t);

    CPURegisters Registers(0);
    Memory memory(&Registers); // Specify the memory size in KB
    CPU cpu(&memory, &Registers);

    // Load BIOS code into the CPU's memory
    //cpu.loadBiosCode(biosCode, numInstructions);
    cpu.loadBIOS("scph1001.bin");
    cpu.loadInstructions();

    // Run the CPU to execute the loaded BIOS code
    cpu.run();
}
