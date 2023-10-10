/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "GPU.h"

GPU::GPU() {
    GPU_status.value = 0x14802000;

    cpu_to_gpu.active = false;
    gpu_to_cpu.active = false;

    command = GPUCommand::None;
}

uint32_t GPU::read(uint32_t address) {
    // Empty Implementation
}

void GPU::write(uint32_t address, uint32_t data) {
    // Empty Implementation
}

uint32_t GPU::get_gpuread() {
    // Empty Implementation
}

uint32_t GPU::get_gpustat() {
    // Empty Implementation
}