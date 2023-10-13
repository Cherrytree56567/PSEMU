/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "GPU.h"

int GPU::command_size[16 * 16] {
                //0  1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
                1,  1,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //0
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //1
                4,  4,  4,  4,  7,  7,  7,  7,  5,  5,  5,  5,  9,  9,  9,  9, //2
                6,  6,  6,  6,  9,  9,  9,  9,  8,  8,  8,  8, 12, 12, 12, 12, //3
                3,  3,  3,  3,  3,  3,  3,  3, 16, 16, 16, 16, 16, 16, 16, 16, //4
                4,  4,  4,  4,  4,  4,  4,  4, 16, 16, 16, 16, 16, 16, 16, 16, //5
                3,  3,  3,  1,  4,  4,  4,  4,  2,  1,  2,  1,  3,  3,  3,  3, //6
                2,  1,  2,  1,  3,  3,  3,  3,  2,  1,  2,  2,  3,  3,  3,  3, //7
                4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, //8
                4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, //9
                3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, //A
                3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, //B
                3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, //C
                3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, //D
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, //E
                1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  //F
        };

void GPU::write_gp0(uint32_t data) {
    if (cpu_to_gpu.active) {
        auto lower = uint16_t(data >> 0);
        auto upper = uint16_t(data >> 16);

        vram_transfer(lower);
        vram_transfer(upper);
        return;
    }

    /* Push command word in the fifo. */
    fifo.push_back(data);
    uint32_t commanda = fifo[0] >> 24;

    /* If the command is complete, execute it. */
    if (fifo.size() == command_size[commanda]) {
        if (commanda == 0x00) {
            gp0_nop();
            command = GPUCommand::Nop;
        }
        else if (commanda == 0x01) {
            gp0_clear_cache();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0x02) {
            gp0_fill_rect();
            command = GPUCommand::Fill_Rectangle;
        }
        else if (commanda >= 0x20 && commanda <= 0x3F) {
            gp0_render_polygon();
            command = GPUCommand::Polygon;
        }
        else if (commanda >= 0x40 && commanda <= 0x5F) {
            command = GPUCommand::Line;
        }
        else if (commanda >= 0x60 && commanda <= 0x7F) {
            gp0_render_rect();
            command = GPUCommand::Rectangle;
        }
        else if (commanda >= 0x80 && commanda <= 0x9F) {
            gp0_image_transfer();
            command = GPUCommand::Vram_Vram;
        }
        else if (commanda >= 0xA0 && commanda <= 0xBF) {
            gp0_image_load();
            command = GPUCommand::Cpu_Vram;
        }
        else if (commanda >= 0xC0 && commanda <= 0xDF) {
            gp0_image_store();
            command = GPUCommand::Vram_Cpu;
        }
        else if (commanda == 0xE1) {
            gp0_draw_mode();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0xE2) {
            gp0_texture_window_setting();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0xE3) {
            gp0_draw_area_top_left();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0xE4) {
            gp0_draw_area_bottom_right();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0xE5) {
            gp0_drawing_offset();
            command = GPUCommand::Render_Attrib;
        }
        else if (commanda == 0xE6) {
            gp0_mask_bit_setting();
            command = GPUCommand::Render_Attrib;
        }
        else {
            printf("[GPU] write_gp0: unknown command: 0x%x\n", commanda);
            exit(1);
        }

        /* Do not forget to clear the fifo! */
        fifo.clear();
    }
}

void GPU::gp0_nop() {
    return;
}

void GPU::gp0_fill_rect() {
    auto color = extract_color(fifo[0]);
    auto top_left = extract_point(fifo[1]);
    auto size = extract_point(fifo[2]);

    glm::ivec2 points[4] =
            {
                    glm::ivec2(0), glm::ivec2(size.x, 0),
                    glm::ivec2(0, size.y), size
            };

    /* Generate vertex data. */
    for (auto& p : points) {
        auto vertex = top_left + p;

        Vertex v = {};
        v.color = color;
        v.pos = vertex;
        v.textured = false;

        vertexData.push_back(v);
    }

    /* Complete the quad. */
    vertexData.insert(vertexData.end(), { vertexData[1], vertexData[2] });

    /* Force draw. */
    /* NOTE: this done as fill commands ignore all */
    /* mask settings that the batch renderer uses. */
    //gl_renderer->draw(vertexData);
    vertexData.clear();
}

void GPU::gp0_draw_mode() {
    uint32_t val = fifo[0];

    GPU_status.page_base_x = (uint8_t)(val & 0xF);
    GPU_status.page_base_y = (uint8_t)((val >> 4) & 0x1);
    GPU_status.semi_transprency = (uint8_t)((val >> 5) & 0x3);
    GPU_status.texture_depth = (uint8_t)((val >> 7) & 0x3);
    GPU_status.dithering = ((val >> 9) & 0x1) != 0;
    GPU_status.draw_to_display = ((val >> 10) & 0x1) != 0;
    GPU_status.texture_disable = ((val >> 11) & 0x1) != 0;
    textured_rectangle_flip.x = ((val >> 12) & 0x1) != 0;
    textured_rectangle_flip.y = ((val >> 13) & 0x1) != 0;
}

void GPU::gp0_draw_area_top_left() {
    // Empty Implementation
}

void GPU::gp0_draw_area_bottom_right() {
    // Empty Implementation
}

void GPU::gp0_texture_window_setting() {
    // Empty Implementation
}

void GPU::gp0_drawing_offset() {
    // Empty Implementation
}

void GPU::gp0_mask_bit_setting() {
    // Empty Implementation
}

void GPU::gp0_clear_cache() {
    // Empty Implementation
}

void GPU::gp0_image_load() {
    // Empty Implementation
}

void GPU::gp0_image_store() {
    // Empty Implementation
}

void GPU::gp0_image_transfer() {
    // Empty Implementation
}

void GPU::gp0_render_polygon() {
    // Empty Implementation
}

void GPU::gp0_render_rect() {
    // Empty Implementation
}

void GPU::gp0_render_line() {
    // Empty Implementation
}