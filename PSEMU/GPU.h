/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include "VRAM.h"
#include "glm/glm/glm.hpp"

union GPUSTATUS {
    uint32_t value;

    struct {
        uint32_t page_base_x : 4;
        uint32_t page_base_y : 1;
        uint32_t semi_transprency : 2;
        uint32_t texture_depth : 2;
        uint32_t dithering : 1;
        uint32_t draw_to_display : 1;
        uint32_t force_set_mask_bit : 1;
        uint32_t preserve_masked_pixels : 1;
        uint32_t field : 1;
        uint32_t reverse_flag : 1;
        uint32_t texture_disable : 1;

        union {
            uint32_t hres : 3;

            struct { uint32_t hres2 : 1; uint32_t hres1 : 2; };
        };

        uint32_t vres : 1;
        uint32_t video_mode : 1;
        uint32_t color_depth : 1;
        uint32_t vertical_interlace : 1;
        uint32_t display_disable : 1;
        uint32_t interrupt_req : 1;
        uint32_t dma_request : 1;
        uint32_t ready_cmd : 1;
        uint32_t ready_vram : 1;
        uint32_t ready_dma : 1;
        uint32_t dma_dir : 2;
        uint32_t odd_lines : 1;
    };
};

struct DatMov {
    uint8_t start_x, start_y;
    uint8_t width, height;
    uint8_t pos_x, pos_y;
    bool active;
};

enum class GPUCommand : uint32_t {
    Polygon,
    Rectangle,
    Fill_Rectangle,
    Line,
    Nop,
    Render_Attrib,
    Cpu_Vram,
    Vram_Cpu,
    Vram_Vram,
    None
};

class GPU {
public:
    GPU();

    uint32_t read(uint32_t address);
    void write(uint32_t address, uint32_t data);

    /* GPU memory read/write commands. */
    void write_gp0(uint32_t data);
    void write_gp1(uint32_t data);
    uint32_t get_gpuread();
    uint32_t get_gpustat();

    /* GP0 commands. */
    void gp0_nop();
    void gp0_fill_rect();
    void gp0_draw_mode();
    void gp0_draw_area_top_left();
    void gp0_draw_area_bottom_right();
    void gp0_texture_window_setting();
    void gp0_drawing_offset();
    void gp0_mask_bit_setting();
    void gp0_clear_cache();
    void gp0_image_load();
    void gp0_image_store();
    void gp0_image_transfer();

    /* GP0 registers. */
    glm::u8vec2 texture_window_mask;
    glm::u8vec2 texture_window_offset;
    glm::u16vec2 drawing_area_top_left;
    glm::u16vec2 drawing_area_bottom_right;
    glm::i16vec2 draw_offset;

    /* GP1 registers. */
    glm::u16vec2 display_area;
    glm::u16vec2 display_area_top_left;
    glm::u16vec2 display_area_bottom_right;
    glm::bvec2 textured_rectangle_flip;

    DatMov cpu_to_gpu, gpu_to_cpu;
    GPUCommand command;
    GPUSTATUS GPU_status;

    int height[2] = { 240, 480 };
    int depth[4] = { 4, 8, 16, 0 };
    int width[7] = { 256, 368, 320, 0, 512, 0, 640 };
    int dotClockDiv[5] = { 10, 8, 5, 4, 7 };

    static int command_size[16 * 16];

    std::vector<uint32_t> fifo;
};