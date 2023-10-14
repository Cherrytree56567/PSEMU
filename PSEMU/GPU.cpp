/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "GPU.h"

template<int bi>
uint32_t sl(uint32_t value) {
    enum { mask = (1 << bi) - 1 };
    enum { sign = 1 << (bi - 1) };

    return ((value & mask) ^ sign) - sign;
}

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
    return 0;
}

glm::ivec3 GPU::extract_color(uint32_t color) {
    glm::ivec3 result;
    result.r = (color >> 0) & 0xff;
    result.g = (color >> 8) & 0xff;
    result.b = (color >> 16) & 0xff;

    return result;
}
glm::ivec2 GPU::extract_point(uint32_t point) {
    glm::ivec2 result;
    result.x = sl<11>(point);
    result.y = sl<11>(point >> 16);

    return result;
}
glm::ivec2 GPU::extract_coord(uint32_t coord) {
    glm::ivec2 p;
    p.s = (coord >> 0) & 0xff;
    p.t = (coord >> 8) & 0xff;

    return p;
}

bool GPU::tick(uint32_t cycles) {
    std::cout << "Warning: GPU tick is not implemented.";
    return false;
}