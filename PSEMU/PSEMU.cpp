/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include <iostream>
#include "CPU.h"
#include "Memory.h"
#include "Logging.h"

/*
 * Somehow get the DMA Working.
 * The DMA needs access to the register and the memory so if put DMA in the CPU and then pass the memory and registers I can allow DMA to work.
 * That was the first part but the second part is to be able to map it.
 *
 * Also, All the CPU Registers have the same value somehow. EDIT: nvm I fixed it, turns out I hadn't initialized all the CPURegister array to 0.
 *
 * Ok, I found a way to fix the DMA Circular Dependencies, I can just merge the DMA with the Memory.
 * I finally got it working.
 * 
 * Priorities:
 * [ ] Get GPU working
 * [ ] Make the Renderer and look at these functions that use it
 * - gp0_fill_rect
 * [X] Implement Tick.
 * [ ] Add Proper Memory
 * 
 * Just a note, I'm not sure if it's nessairy or not but other emulators seem to be starting at a specific address but mine just starts at zero.
 * Also I think we need to use a tick
 * I think the j instruction at the end uses the DMA.
 * 
 * NOTE: The gpu tick is not implemented
 */

int main() {
    uint32_t biosCode[] = {
        0b00111100000000010000000000000001,
        0b00000000001000010000100000100100,
    };
    size_t numInstructions = sizeof(biosCode) / sizeof(uint32_t);

    CPURegisters Registers(0);
    Memory memory(2048, &Registers); // Specify the memory size in KB
    CPU cpu(&memory, &Registers);

    // Load BIOS code into the CPU's memory
    //cpu.loadBiosCode(biosCode, numInstructions);
    cpu.loadBIOS("scph1001.bin");
    cpu.loadInstructions();

    memory.control = 0x07654321;

    // Run the CPU to execute the loaded BIOS code
    while (true) {
      cpu.tick();
      cpu.handleInterrupts();
      
      memory.tick(); // dMA

      /*if (memory.gpu.tick(300)) {
          // Empty
      }  */
    }
}
