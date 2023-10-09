/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <cstdint>
#include "glm/glm/vec3.hpp"
#include "glm/glm/vec2.hpp"
#include "glm/glm/vec4.hpp"
#include "Logging.h"
#include <tuple>

namespace glm {
    typedef mat<3, 3, int16_t> i16mat3;
}

union Color {
    uint32_t val;
    glm::u8vec4 color;
    struct { int8_t r, g, b, c; };
};

union Vec2i16 {
    uint32_t val;
    struct { int16_t x, y; };
    glm::i16vec2 vector;

    auto& operator[](int idx) { return vector[idx]; }
};

union Vec3i16 {
    uint32_t xy;
    struct { int16_t x, y, z; };
    glm::i16vec3 vector;

    auto& operator[](int idx) { return vector[idx]; }
};

struct Matrix {
    Vec3i16 v1, v2, v3;
};

union GTECommand {
    uint32_t raw = 0;

    struct {
        uint32_t opcode : 6;
        uint32_t : 4;
        uint32_t lm : 1;
        uint32_t : 2;
        uint32_t mvmva_tr_vec : 2;
        uint32_t mvmva_mul_vec : 2;
        uint32_t mvmva_mul_matrix : 2;
        uint32_t sf : 1;
        uint32_t fake_opcode : 5;
        uint32_t : 7;
    };
};

class GTE {
public:
    GTE();

    // GTE Data Registers
    Vec3i16 V[3];   //R0-1 R2-3 R4-5 s16
    Color RGBC;                     //R6
    uint16_t OTZ;                     //R7
    short IR[4];      //R8-11
    Vec2i16 SXY[4]; //R12-15 FIFO
    uint16_t SZ[4];    //R16-19 FIFO
    Color RGB[3];     //R20-22 FIFO
    uint32_t RES1;                      //R23 prohibited
    int MAC0;                       //R24
    int MAC1, MAC2, MAC3;           //R25-27
    uint16_t IRGB;//, ORGB;           //R28-29 Orgb is readonly and read by irgb
    int LZCS, LZCR;                 //R30-31

    //Control Registers
    Matrix RT, LM, LRGB;        //R32-36 R40-44 R48-52
    int TRX, TRY, TRZ;          //R37-39
    int RBK, GBK, BBK;          //R45-47
    int RFC, GFC, BFC;          //R53-55
    int OFX, OFY, DQB;          //R56 57 60
    uint16_t H;                   //R58
    short ZSF3, ZSF4, DQA;      //R61 62 59
    uint32_t FLAG;

    //Debuggers
    Logging console;

    // GTE commands
    void op_rtps(uint32_t command);
    void op_nclip(uint32_t command);
    void op_op(uint32_t command);
    void op_mvmva(uint32_t command);
    void op_dpcs(uint32_t command);
    void op_intpl(uint32_t command);
    void op_cc(uint32_t command);
    void op_rtpt(uint32_t command);
    void op_ncds(uint32_t command);
    void op_avsz3(uint32_t command);
    void op_ncct(uint32_t command);
    void op_gpf(uint32_t command);
    void op_sqr(uint32_t command);
    void op_avsz4(uint32_t command);
    void op_nct(uint32_t command);
    void op_ncdt(uint32_t command);
    void op_nccs(uint32_t command);
    void op_ncs(uint32_t command);

    // Set Commands For Flags
    inline int32_t set_mac0_flag(int64_t value);
    inline int64_t set_mac_flag(int num, int64_t value);
    inline uint16_t set_sz3_flag(int64_t value);
    inline int16_t set_sxy_flag(int i, int32_t value);
    inline int16_t set_ir0_flag(int64_t value);
    inline int16_t set_ir_flag(int i, int32_t value, bool lm);
    inline uint8_t set_rgb(int i, int value);

    // Mvmva commands
    Matrix get_mvmva_matrix(uint32_t command);
    Vec3i16 get_mvmva_vector(uint32_t command);
    std::tuple<int, int, int> get_mvmva_translation(uint32_t command);

    void interpolate(int32_t mac1, int32_t mac2, int32_t mac3, uint32_t command);

    // Execute GTE command
    void ExecuteCommand(uint32_t command);
    uint32_t read_data(uint32_t reg);
    void write_data(uint32_t reg, uint32_t v);
    uint32_t read_control(uint32_t reg);
    void write_control(uint32_t reg, uint32_t v);
};
