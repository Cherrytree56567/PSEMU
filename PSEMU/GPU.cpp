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
    return 0;
}

void GPU::write(uint32_t address, uint32_t data) {
    // Empty Implementation
}

uint32_t GPU::get_gpuread() {
    // Empty Implementation
    return 0;
}

uint32_t GPU::get_gpustat() {
    // Empty Implementation
    return 0;
}

void GPU::vram_transfer(uint16_t data) {
    // Empty
}
uint16_t GPU::vram_transfer() {
    // Empty
}

glm::ivec3 GPU::extract_color(uint32_t color) {
    // Empty
}
glm::ivec2 GPU::extract_point(uint32_t point) {
    // Empty
}
glm::ivec2 GPU::extract_coord(uint32_t coord) {
    // Empty
}