/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "GTE.h"

GTE::GTE() {
    // Initialize registers to default values
}

void GTE::ExecuteCommand(uint32_t command) {
    // Decode and execute the GTE command based on its opcode, probably use switch statements just like in the CPU Class
    GTECommand com;
    com.raw = command;
    switch (com.opcode) {
        case 0x1:
            op_rtps(command);
            break;

        case 0x6:
            op_nclip(command);
            break;

        case 0xc:
            op_op(command);
            break;

        case 0x12:
            op_mvmva(command);
            break;

        case 0x10:
            op_dpcs(command);
            break;

        case 0x11:
            op_intpl(command);
            break;

        case 0x1c:
            op_cc(command);
            break;

        case 0x30:
            op_rtpt(command);
            break;

        case 0x13:
            op_ncds(command);
            break;

        case 0x2d:
            op_avsz3(command);
            break;

        case 0x3f:
            op_ncct(command);
            break;

        case 0x3d:
            op_gpf(command);
            break;

        case 0x28:
            op_sqr(command);
            break;

        case 0x2e:
            op_avsz4(command);
            break;

        case 0x20:
            op_nct(command);
            break;

        case 0x16:
            op_ncdt(command);
            break;

        case 0x1b:
            op_nccs(command);
            break;

        case 0x1e:
            op_ncs(command);
            break;
    }
}

uint32_t GTE::read_data(uint32_t reg) {
    console.log("GTE Register read: " + std::to_string(reg) + "\n");

    auto saturateRGB = [&](int v) -> short
    {
        short saturate = (short)v;
        if (saturate < 0x00) return 0x00;
        else if (saturate > 0x1F) return 0x1F;
        else return saturate;
    };

    uint32_t value;
    switch (reg) {
        case 00: value = V[0].xy; break;
        case 01: value = (uint32_t)V[0].z; break;
        case 02: value = V[1].xy; break;
        case 03: value = (uint32_t)V[1].z; break;
        case 04: value = V[2].xy; break;
        case 05: value = (uint32_t)V[2].z; break;
        case 06: value = RGBC.val; break;
        case 07: value = OTZ; break;
        case 8: value = (uint32_t)IR[0]; break;
        case 9: value = (uint32_t)IR[1]; break;
        case 10: value = (uint32_t)IR[2]; break;
        case 11: value = (uint32_t)IR[3]; break;
        case 12: value = SXY[0].val; break;
        case 13: value = SXY[1].val; break;
        case 14: //Mirror
        case 15: value = SXY[2].val; break;
        case 16: value = SZ[0]; break;
        case 17: value = SZ[1]; break;
        case 18: value = SZ[2]; break;
        case 19: value = SZ[3]; break;
        case 20: value = RGB[0].val; break;
        case 21: value = RGB[1].val; break;
        case 22: value = RGB[2].val; break;
        case 23: value = RES1; break; //Prohibited Register
        case 24: value = (uint32_t)MAC0; break;
        case 25: value = (uint32_t)MAC1; break;
        case 26: value = (uint32_t)MAC2; break;
        case 27: value = (uint32_t)MAC3; break;
        case 28:/* value = IRGB; break;*/
        case 29:/* value = ORGB; break;*/
            IRGB = (uint16_t)(saturateRGB(IR[3] / 0x80) << 10 | saturateRGB(IR[2] / 0x80) << 5 | (uint16_t)saturateRGB(IR[1] / 0x80));
            value = IRGB;
            break;
        case 30: value = (uint32_t)LZCS; break;
        case 31: value = (uint32_t)LZCR; break;
        default: value = 0xFFFFFFFF; break;
    }
    return value;
}

void GTE::write_data(uint32_t reg, uint32_t v) {
    console.log("GTE Register write: " + std::to_string(reg) + " with data: " + std::to_string(v) + "\n");

    auto leadingCount = [&](uint32_t v) -> int
    {
        uint32_t sign = (v >> 31);
        int leadingCount = 0;
        for (int i = 0; i < 32; i++) {
            if (v >> 31 != sign) break;
            leadingCount++;
            v <<= 1;
        }
        return leadingCount;
    };

    switch (reg) {
        case 00: V[0].xy = v; break;
        case 01: V[0].z = (short)v; break;
        case 02: V[1].xy = v; break;
        case 03: V[1].z = (short)v; break;
        case 04: V[2].xy = v; break;
        case 05: V[2].z = (short)v; break;
        case 06: RGBC.val = v; break;
        case 07: OTZ = (uint16_t)v; break;
        case 8: IR[0] = (short)v; break;
        case 9: IR[1] = (short)v; break;
        case 10: IR[2] = (short)v; break;
        case 11: IR[3] = (short)v; break;
        case 12: SXY[0].val = v; break;
        case 13: SXY[1].val = v; break;
        case 14: SXY[2].val = v; break;
        case 15: SXY[0] = SXY[1]; SXY[1] = SXY[2]; SXY[2].val = v; break; //On load mirrors 0x14 on write cycles the fifo
        case 16: SZ[0] = (uint16_t)v; break;
        case 17: SZ[1] = (uint16_t)v; break;
        case 18: SZ[2] = (uint16_t)v; break;
        case 19: SZ[3] = (uint16_t)v; break;
        case 20: RGB[0].val = v; break;
        case 21: RGB[1].val = v; break;
        case 22: RGB[2].val = v; break;
        case 23: RES1 = v; break;
        case 24: MAC0 = (int)v; break;
        case 25: MAC1 = (int)v; break;
        case 26: MAC2 = (int)v; break;
        case 27: MAC3 = (int)v; break;
        case 28:
            IRGB = (uint16_t)(v & 0x7FFF);
            IR[1] = (short)((v & 0x1F) * 0x80);
            IR[2] = (short)(((v >> 5) & 0x1F) * 0x80);
            IR[3] = (short)(((v >> 10) & 0x1F) * 0x80);
            break;
        case 29: /*ORGB = (ushort)v;*/ break; //Only Read its set by IRGB
        case 30: LZCS = (int)v; LZCR = leadingCount(v); break;
        case 31: /*LZCR = (int)v;*/ break; //Only Read its set by LZCS
    }
}

uint32_t GTE::read_control(uint32_t reg) {
    uint32_t value;
    switch (reg) {
        case 00: value = RT.v1.xy; break;
        case 01: value = (uint16_t)RT.v1.z | (uint32_t)(RT.v2.x << 16); break;
        case 02: value = (uint16_t)RT.v2.y | (uint32_t)(RT.v2.z << 16); break;
        case 03: value = RT.v3.xy; break;
        case 04: value = (uint32_t)RT.v3.z; break;
        case 05: value = (uint32_t)TRX; break;
        case 06: value = (uint32_t)TRY; break;
        case 07: value = (uint32_t)TRZ; break;
        case 8: value = LM.v1.xy; break;
        case 9: value = (uint16_t)LM.v1.z | (uint32_t)(LM.v2.x << 16); break;
        case 10: value = (uint16_t)LM.v2.y | (uint32_t)(LM.v2.z << 16); break;
        case 11: value = LM.v3.xy; break;
        case 12: value = (uint32_t)LM.v3.z; break;
        case 13: value = (uint32_t)RBK; break;
        case 14: value = (uint32_t)GBK; break;
        case 15: value = (uint32_t)BBK; break;
        case 16: value = LRGB.v1.xy; break;
        case 17: value = (uint16_t)LRGB.v1.z | (uint32_t)(LRGB.v2.x << 16); break;
        case 18: value = (uint16_t)LRGB.v2.y | (uint32_t)(LRGB.v2.z << 16); break;
        case 19: value = LRGB.v3.xy; break;
        case 20: value = (uint32_t)LRGB.v3.z; break;
        case 21: value = (uint32_t)RFC; break;
        case 22: value = (uint32_t)GFC; break;
        case 23: value = (uint32_t)BFC; break;
        case 24: value = (uint32_t)OFX; break;
        case 25: value = (uint32_t)OFY; break;
        case 26: value = (uint32_t)(short)H; break; //sign extend
        case 27: value = (uint32_t)DQA; break;
        case 28: value = (uint32_t)DQB; break;
        case 29: value = (uint32_t)ZSF3; break; //sign extend
        case 30: value = (uint32_t)ZSF4; break; //sign extend
        case 31: value = FLAG; break;
        default: value = 0xFFFFFFFF; break;
    }
    return value;
}

void GTE::write_control(uint32_t reg, uint32_t v) {
    switch (reg) {
        case 00: RT.v1.xy = v; break;
        case 01: RT.v1.z = (short)v; RT.v2.x = (short)(v >> 16); break;
        case 02: RT.v2.y = (short)v; RT.v2.z = (short)(v >> 16); break;
        case 03: RT.v3.xy = v; break;
        case 04: RT.v3.z = (short)v; break;
        case 05: TRX = (int)v; break;
        case 06: TRY = (int)v; break;
        case 07: TRZ = (int)v; break;
        case 8: LM.v1.xy = v; break;
        case 9: LM.v1.z = (short)v; LM.v2.x = (short)(v >> 16); break;
        case 10: LM.v2.y = (short)v; LM.v2.z = (short)(v >> 16); break;
        case 11: LM.v3.xy = v; break;
        case 12: LM.v3.z = (short)v; break;
        case 13: RBK = (int)v; break;
        case 14: GBK = (int)v; break;
        case 15: BBK = (int)v; break;
        case 16: LRGB.v1.xy = v; break;
        case 17: LRGB.v1.z = (short)v; LRGB.v2.x = (short)(v >> 16); break;
        case 18: LRGB.v2.y = (short)v; LRGB.v2.z = (short)(v >> 16); break;
        case 19: LRGB.v3.xy = v; break;
        case 20: LRGB.v3.z = (short)v; break;
        case 21: RFC = (int)v; break;
        case 22: GFC = (int)v; break;
        case 23: BFC = (int)v; break;
        case 24: OFX = (int)v; break;
        case 25: OFY = (int)v; break;
        case 26: H = (uint16_t)v; break;
        case 27: DQA = (short)v; break;
        case 28: DQB = (int)v; break;
        case 29: ZSF3 = (short)v; break;
        case 30: ZSF4 = (short)v; break;
        case 31: //flag is u20 with 31 Error Flag (Bit30..23, and 18..13 ORed together)
            FLAG = v & 0x7FFFF000;
            if ((FLAG & 0x7F87E000) != 0) {
                FLAG |= 0x80000000;
            }
            break;
    }
}

void GTE::op_rtps(uint32_t command) {
    GTECommand com;
    com.raw = command;
    int i = 0;

    MAC1 = (int)set_mac_flag(1, (TRX * 0x1000 + RT.v1.x * V[i].x + RT.v1.y * V[i].y + RT.v1.z * V[i].z) >> (com.sf * 12));
    MAC2 = (int)set_mac_flag(2, (TRY * 0x1000 + RT.v2.x * V[i].x + RT.v2.y * V[i].y + RT.v2.z * V[i].z) >> (com.sf * 12));
    MAC3 = (int)set_mac_flag(3, (TRZ * 0x1000 + RT.v3.x * V[i].x + RT.v3.y * V[i].y + RT.v3.z * V[i].z) >> (com.sf * 12));

    IR[1] = set_ir_flag(1, MAC1, false);
    IR[2] = set_ir_flag(2, MAC2, false);
    IR[3] = set_ir_flag(3, MAC3, false);

    //SZ3 = MAC3 SAR ((1-command.)*12)                           ;ScreenZ FIFO 0..+FFFFh

    SZ[0] = SZ[1];
    SZ[1] = SZ[2];
    SZ[2] = SZ[3];
    SZ[3] = set_sz3_flag(MAC3 >> ((1 - com.sf) * 12));

    SXY[0] = SXY[1];
    SXY[1] = SXY[2];

    int64_t result;
    int64_t div = 0;
    if (SZ[3] == 0) {
        result = 0x1FFFF;
    }
    else {
        div = (((int64_t)H * 0x20000 / SZ[3]) + 1) / 2;

        if (div > 0x1FFFF) {
            result = 0x1FFFF;
            FLAG |= 0x1 << 17;
        }
        else {
            result = div;
        }
    }

    MAC0 = (int)(result * IR[1] + OFX);
    SXY[2].x = set_sxy_flag(2, MAC0 / 0x10000);
    MAC0 = (int)(result * IR[2] + OFY);
    SXY[2].y = set_sxy_flag(2, MAC0 / 0x10000);
    MAC0 = (int)(result * DQA + DQB);
    IR[0] = set_ir0_flag(MAC0 / 0x1000);

}

void GTE::op_nclip(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC0 = set_mac0_flag((int64_t)SXY[0].x * SXY[1].y + SXY[1].x * SXY[2].y + SXY[2].x * SXY[0].y - SXY[0].x * SXY[2].y - SXY[1].x * SXY[0].y - SXY[2].x * SXY[1].y);
}

void GTE::op_op(uint32_t command) {
    GTECommand com;
    com.raw = command;

    short d1 = RT.v1.x;
    short d2 = RT.v2.y;
    short d3 = RT.v3.z;

    MAC1 = (int)set_mac_flag(1, ((IR[3] * d2) - (IR[2] * d3)) >> com.sf * 12);
    MAC2 = (int)set_mac_flag(2, ((IR[1] * d3) - (IR[3] * d1)) >> com.sf * 12);
    MAC3 = (int)set_mac_flag(3, ((IR[2] * d1) - (IR[1] * d2)) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}

void GTE::op_mvmva(uint32_t command) {
    GTECommand com;
    com.raw = command;

    Matrix mx = get_mvmva_matrix(command);
    Vec3i16 vx = get_mvmva_vector(command);
    auto [tx, ty, tz] = get_mvmva_translation(command);

    MAC1 = (int)set_mac_flag(1, (int64_t)(tx * 0x1000 + mx.v1.x * vx.x + mx.v1.y * vx.y + mx.v1.z * vx.z) >> com.sf * 12);
    MAC2 = (int)set_mac_flag(2, (int64_t)(ty * 0x1000 + mx.v2.x * vx.x + mx.v2.y * vx.y + mx.v2.z * vx.z) >> com.sf * 12);
    MAC3 = (int)set_mac_flag(3, (int64_t)(tz * 0x1000 + mx.v3.x * vx.x + mx.v3.y * vx.y + mx.v3.z * vx.z) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}

void GTE::op_dpcs(uint32_t command) {
    GTECommand com;
    com.raw = command;

    uint32_t r = RGBC.r;
    uint32_t g = RGBC.g;
    uint32_t b = RGBC.b;

    // WHEN DCPT it uses RGB FIFO instead RGBC
    //[MAC1, MAC2, MAC3] = [R, G, B] SHL 16                     ;<--- for DPCS/DPCT
    MAC1 = (int)(set_mac_flag(1, r) << 16);
    MAC2 = (int)(set_mac_flag(2, g) << 16);
    MAC3 = (int)(set_mac_flag(3, b) << 16);

    interpolate(MAC1, MAC2, MAC3, command);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;
}

void GTE::op_intpl(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)set_mac_flag(1, (int64_t)IR[1] << 12);
    MAC2 = (int)set_mac_flag(2, (int64_t)IR[2] << 12);
    MAC3 = (int)set_mac_flag(3, (int64_t)IR[3] << 12);

    interpolate(MAC1, MAC2, MAC3, command);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;
}

void GTE::op_cc(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (int64_t)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (int64_t)LRGB.v1.y * IR[2]) + (int64_t)LRGB.v1.z * IR[3]) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (int64_t)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (int64_t)LRGB.v2.y * IR[2]) + (int64_t)LRGB.v2.z * IR[3]) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (int64_t)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (int64_t)LRGB.v3.y * IR[2]) + (int64_t)LRGB.v3.z * IR[3]) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // [MAC1, MAC2, MAC3] = [R * IR1, G * IR2, B * IR3] SHL 4;
    MAC1 = (int)(set_mac_flag(1, (int64_t)RGBC.r * IR[1]) << 4);
    MAC2 = (int)(set_mac_flag(2, (int64_t)RGBC.g * IR[2]) << 4);
    MAC3 = (int)(set_mac_flag(3, (int64_t)RGBC.b * IR[3]) << 4);

    // [MAC1, MAC2, MAC3] = [MAC1, MAC2, MAC3] SAR(command.sf * 12);< --- for NCDx / NCCx
    MAC1 = (int)(set_mac_flag(1, MAC1) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, MAC2) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, MAC3) >> com.sf * 12);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE], [IR1, IR2, IR3] = [MAC1, MAC2, MAC3]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}

void GTE::op_rtpt(uint32_t command) {
    GTECommand com;
    com.raw = command;

    for (int i = 0; i < 3; i++) {
        MAC1 = (int)set_mac_flag(1, (TRX * 0x1000 + RT.v1.x * V[i].x + RT.v1.y * V[i].y + RT.v1.z * V[i].z) >> (com.sf * 12));
        MAC2 = (int)set_mac_flag(2, (TRY * 0x1000 + RT.v2.x * V[i].x + RT.v2.y * V[i].y + RT.v2.z * V[i].z) >> (com.sf * 12));
        MAC3 = (int)set_mac_flag(3, (TRZ * 0x1000 + RT.v3.x * V[i].x + RT.v3.y * V[i].y + RT.v3.z * V[i].z) >> (com.sf * 12));

        IR[1] = set_ir_flag(1, MAC1, false);
        IR[2] = set_ir_flag(2, MAC2, false);
        IR[3] = set_ir_flag(3, MAC3, false);

        SZ[0] = SZ[1];
        SZ[1] = SZ[2];
        SZ[2] = SZ[3];
        SZ[3] = set_sz3_flag(MAC3 >> ((1 - com.sf) * 12));

        SXY[0] = SXY[1];
        SXY[1] = SXY[2];

        int64_t result;
        int64_t div = 0;
        if (SZ[3] == 0) {
            result = 0x1FFFF;
        }
        else {
            div = (((int64_t)H * 0x20000 / SZ[3]) + 1) / 2;

            if (div > 0x1FFFF) {
                result = 0x1FFFF;
                FLAG |= 0x1 << 17;
            }
            else {
                result = div;
            }
        }

        MAC0 = (int)(result * IR[1] + OFX);
        SXY[2].x = set_sxy_flag(2, MAC0 / 0x10000);
        MAC0 = (int)(result * IR[2] + OFY);
        SXY[2].y = set_sxy_flag(2, MAC0 / 0x10000);
        MAC0 = (int)(result * DQA + DQB);
        IR[0] = set_ir0_flag(MAC0 / 0x1000);
    }
}

void GTE::op_ncds(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)(set_mac_flag(1, (int64_t)LM.v1.x * V[0].x + LM.v1.y * V[0].y + LM.v1.z * V[0].z) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, (int64_t)LM.v2.x * V[0].x + LM.v2.y * V[0].y + LM.v2.z * V[0].z) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, (int64_t)LM.v3.x * V[0].x + LM.v3.y * V[0].y + LM.v3.z * V[0].z) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (int64_t)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (int64_t)LRGB.v1.y * IR[2]) + (int64_t)LRGB.v1.z * IR[3]) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (int64_t)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (int64_t)LRGB.v2.y * IR[2]) + (int64_t)LRGB.v2.z * IR[3]) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (int64_t)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (int64_t)LRGB.v3.y * IR[2]) + (int64_t)LRGB.v3.z * IR[3]) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // [MAC1, MAC2, MAC3] = [R * IR1, G * IR2, B * IR3] SHL 4;< --- for NCDx / NCCx
    MAC1 = (int)set_mac_flag(1, ((int64_t)RGBC.r * IR[1]) << 4);
    MAC2 = (int)set_mac_flag(2, ((int64_t)RGBC.g * IR[2]) << 4);
    MAC3 = (int)set_mac_flag(3, ((int64_t)RGBC.b * IR[3]) << 4);

    interpolate(MAC1, MAC2, MAC3, command);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;
}

void GTE::op_avsz3(uint32_t command) {
    GTECommand com;
    com.raw = command;

    int64_t avsz3 = (int64_t)ZSF3 * (SZ[1] + SZ[2] + SZ[3]);
    MAC0 = set_mac0_flag(avsz3);
    OTZ = set_sz3_flag(avsz3 >> 12);
}

void GTE::op_ncct(uint32_t command) {
    GTECommand com;
    com.raw = command;

    for (int r = 0; r < 3; r++) {
        MAC1 = (int)(set_mac_flag(1, (int64_t)LM.v1.x * V[r].x + LM.v1.y * V[r].y + LM.v1.z * V[r].z) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, (int64_t)LM.v2.x * V[r].x + LM.v2.y * V[r].y + LM.v2.z * V[r].z) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, (int64_t)LM.v3.x * V[r].x + LM.v3.y * V[r].y + LM.v3.z * V[r].z) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (int64_t)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (int64_t)LRGB.v1.y * IR[2]) + (int64_t)LRGB.v1.z * IR[3]) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (int64_t)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (int64_t)LRGB.v2.y * IR[2]) + (int64_t)LRGB.v2.z * IR[3]) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (int64_t)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (int64_t)LRGB.v3.y * IR[2]) + (int64_t)LRGB.v3.z * IR[3]) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        // [MAC1, MAC2, MAC3] = [R * IR1, G * IR2, B * IR3] SHL 4;< --- for NCDx / NCCx
        MAC1 = (int)set_mac_flag(1, (RGBC.r * IR[1]) << 4);
        MAC2 = (int)set_mac_flag(2, (RGBC.g * IR[2]) << 4);
        MAC3 = (int)set_mac_flag(3, (RGBC.b * IR[3]) << 4);

        // [MAC1, MAC2, MAC3] = [MAC1, MAC2, MAC3] SAR(command.sf * 12);< --- for NCDx / NCCx
        MAC1 = (int)set_mac_flag(1, MAC1 >> com.sf * 12);
        MAC2 = (int)set_mac_flag(2, MAC2 >> com.sf * 12);
        MAC3 = (int)set_mac_flag(3, MAC3 >> com.sf * 12);

        // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE], [IR1, IR2, IR3] = [MAC1, MAC2, MAC3]
        RGB[0] = RGB[1];
        RGB[1] = RGB[2];

        RGB[2].r = set_rgb(1, MAC1 >> 4);
        RGB[2].g = set_rgb(2, MAC2 >> 4);
        RGB[2].b = set_rgb(3, MAC3 >> 4);
        RGB[2].c = RGBC.c;

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);
    }
}

void GTE::op_gpf(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)set_mac_flag(1, IR[1] * IR[0]) >> com.sf * 12;
    MAC2 = (int)set_mac_flag(2, IR[2] * IR[0]) >> com.sf * 12;
    MAC3 = (int)set_mac_flag(3, IR[3] * IR[0]) >> com.sf * 12;

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;
}

void GTE::op_sqr(uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)set_mac_flag(1, (IR[1] * IR[1]) >> com.sf * 12);
    MAC2 = (int)set_mac_flag(2, (IR[2] * IR[2]) >> com.sf * 12);
    MAC3 = (int)set_mac_flag(3, (IR[3] * IR[3]) >> com.sf * 12);


    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}

void GTE::op_avsz4(uint32_t command) {
    GTECommand com;
    com.raw = command;

    long long avsz4 = (long long)ZSF4 * (SZ[0] + SZ[1] + SZ[2] + SZ[3]);
    MAC0 = set_mac0_flag(avsz4);
    OTZ = set_sz3_flag(avsz4 >> 12);
}

void GTE::op_nct(uint32_t command) {
    GTECommand com;
    com.raw = command;

    for (int r = 0; r < 3; r++) {
        MAC1 = (int)(set_mac_flag(1, (long long)LM.v1.x * V[r].x + LM.v1.y * V[r].y + LM.v1.z * V[r].z) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, (long long)LM.v2.x * V[r].x + LM.v2.y * V[r].y + LM.v2.z * V[r].z) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, (long long)LM.v3.x * V[r].x + LM.v3.y * V[r].y + LM.v3.z * V[r].z) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        // [IR1, IR2, IR3] = [MAC1, MAC2, MAC3] = (BK * 1000h + LCM * IR) SAR(sf * 12)
        // WARNING each multiplication can trigger mac flags so the check is needed on each op! Somehow this only affects the color matrix and not the light one
        MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (long long)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (long long)LRGB.v1.y * IR[2]) + (long long)LRGB.v1.z * IR[3]) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (long long)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (long long)LRGB.v2.y * IR[2]) + (long long)LRGB.v2.z * IR[3]) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (long long)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (long long)LRGB.v3.y * IR[2]) + (long long)LRGB.v3.z * IR[3]) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE], [IR1, IR2, IR3] = [MAC1, MAC2, MAC3]
        RGB[0] = RGB[1];
        RGB[1] = RGB[2];

        RGB[2].r = set_rgb(1, MAC1 >> 4);
        RGB[2].g = set_rgb(2, MAC2 >> 4);
        RGB[2].b = set_rgb(3, MAC3 >> 4);
        RGB[2].c = RGBC.c;

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);
    }
}

void GTE::op_ncdt(uint32_t command) {
    GTECommand com;
    com.raw = command;

    for (int r = 0; r < 3; r++) {
        MAC1 = (int)(set_mac_flag(1, (int64_t)LM.v1.x * V[r].x + LM.v1.y * V[r].y + LM.v1.z * V[r].z) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, (int64_t)LM.v2.x * V[r].x + LM.v2.y * V[r].y + LM.v2.z * V[r].z) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, (int64_t)LM.v3.x * V[r].x + LM.v3.y * V[r].y + LM.v3.z * V[r].z) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        // [IR1, IR2, IR3] = [MAC1, MAC2, MAC3] = (BK * 1000h + LCM * IR) SAR(command.sf * 12)
        // WARNING each multiplication can trigger mac flags so the check is needed on each op! Somehow this only affects the color matrix and not the light one
        MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (int64_t)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (int64_t)LRGB.v1.y * IR[2]) + (int64_t)LRGB.v1.z * IR[3]) >> com.sf * 12);
        MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (int64_t)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (int64_t)LRGB.v2.y * IR[2]) + (int64_t)LRGB.v2.z * IR[3]) >> com.sf * 12);
        MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (int64_t)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (int64_t)LRGB.v3.y * IR[2]) + (int64_t)LRGB.v3.z * IR[3]) >> com.sf * 12);

        IR[1] = set_ir_flag(1, MAC1, com.lm);
        IR[2] = set_ir_flag(2, MAC2, com.lm);
        IR[3] = set_ir_flag(3, MAC3, com.lm);

        // [MAC1, MAC2, MAC3] = [R * IR1, G * IR2, B * IR3] SHL 4;< --- for NCDx / NCCx
        MAC1 = (int)set_mac_flag(1, ((int64_t)RGBC.r * IR[1]) << 4);
        MAC2 = (int)set_mac_flag(2, ((int64_t)RGBC.g * IR[2]) << 4);
        MAC3 = (int)set_mac_flag(3, ((int64_t)RGBC.b * IR[3]) << 4);

        interpolate(MAC1, MAC2, MAC3, command);

        // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE]
        RGB[0] = RGB[1];
        RGB[1] = RGB[2];

        RGB[2].r = set_rgb(1, MAC1 >> 4);
        RGB[2].g = set_rgb(2, MAC2 >> 4);
        RGB[2].b = set_rgb(3, MAC3 >> 4);
        RGB[2].c = RGBC.c;
    }
}

void GTE::op_nccs(uint32_t command) {
    GTECommand com;
    com.raw = command;

    int r = 0;
    MAC1 = (int)(set_mac_flag(1, (int64_t)LM.v1.x * V[r].x + LM.v1.y * V[r].y + LM.v1.z * V[r].z) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, (int64_t)LM.v2.x * V[r].x + LM.v2.y * V[r].y + LM.v2.z * V[r].z) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, (int64_t)LM.v3.x * V[r].x + LM.v3.y * V[r].y + LM.v3.z * V[r].z) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // [IR1, IR2, IR3] = [MAC1, MAC2, MAC3] = (BK * 1000h + LCM * IR) SAR(command.sf * 12)
    // WARNING each multiplication can trigger mac flags so the check is needed on each op! Somehow this only affects the color matrix and not the light one
    MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (int64_t)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (int64_t)LRGB.v1.y * IR[2]) + (int64_t)LRGB.v1.z * IR[3]) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (int64_t)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (int64_t)LRGB.v2.y * IR[2]) + (int64_t)LRGB.v2.z * IR[3]) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (int64_t)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (int64_t)LRGB.v3.y * IR[2]) + (int64_t)LRGB.v3.z * IR[3]) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // [MAC1, MAC2, MAC3] = [R * IR1, G * IR2, B * IR3] SHL 4;< --- for NCDx / NCCx
    MAC1 = (int)set_mac_flag(1, (RGBC.r * IR[1]) << 4);
    MAC2 = (int)set_mac_flag(2, (RGBC.g * IR[2]) << 4);
    MAC3 = (int)set_mac_flag(3, (RGBC.b * IR[3]) << 4);

    // [MAC1, MAC2, MAC3] = [MAC1, MAC2, MAC3] SAR(command.sf * 12);< --- for NCDx / NCCx
    MAC1 = (int)set_mac_flag(1, MAC1 >> com.sf * 12);
    MAC2 = (int)set_mac_flag(2, MAC2 >> com.sf * 12);
    MAC3 = (int)set_mac_flag(3, MAC3 >> com.sf * 12);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE], [IR1, IR2, IR3] = [MAC1, MAC2, MAC3]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}

void GTE::op_ncs(uint32_t command) {
    GTECommand com;
    com.raw = command;

    int r = 0;
    MAC1 = (int)(set_mac_flag(1, (long long)LM.v1.x * V[r].x + LM.v1.y * V[r].y + LM.v1.z * V[r].z) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, (long long)LM.v2.x * V[r].x + LM.v2.y * V[r].y + LM.v2.z * V[r].z) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, (long long)LM.v3.x * V[r].x + LM.v3.y * V[r].y + LM.v3.z * V[r].z) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // [IR1, IR2, IR3] = [MAC1, MAC2, MAC3] = (BK * 1000h + LCM * IR) SAR(sf * 12)
    // WARNING each multiplication can trigger mac flags so the check is needed on each op! Somehow this only affects the color matrix and not the light one
    MAC1 = (int)(set_mac_flag(1, set_mac_flag(1, set_mac_flag(1, (long long)RBK * 0x1000 + LRGB.v1.x * IR[1]) + (long long)LRGB.v1.y * IR[2]) + (long long)LRGB.v1.z * IR[3]) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, set_mac_flag(2, set_mac_flag(2, (long long)GBK * 0x1000 + LRGB.v2.x * IR[1]) + (long long)LRGB.v2.y * IR[2]) + (long long)LRGB.v2.z * IR[3]) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, set_mac_flag(3, set_mac_flag(3, (long long)BBK * 0x1000 + LRGB.v3.x * IR[1]) + (long long)LRGB.v3.y * IR[2]) + (long long)LRGB.v3.z * IR[3]) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);

    // Color FIFO = [MAC1 / 16, MAC2 / 16, MAC3 / 16, CODE], [IR1, IR2, IR3] = [MAC1, MAC2, MAC3]
    RGB[0] = RGB[1];
    RGB[1] = RGB[2];

    RGB[2].r = set_rgb(1, MAC1 >> 4);
    RGB[2].g = set_rgb(2, MAC2 >> 4);
    RGB[2].b = set_rgb(3, MAC3 >> 4);
    RGB[2].c = RGBC.c;

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}


int64_t GTE::set_mac_flag(int i, int64_t value) {
    if (value < -0x80000000000) {
        //Console.WriteLine("under");
        FLAG |= (uint32_t)(0x8000000 >> (i - 1));
    }

    if (value > 0x7FFFFFFFFFF) {
        //Console.WriteLine("over");
        FLAG |= (uint32_t)(0x40000000 >> (i - 1));
    }

    return (value << 20) >> 20;
}

int32_t GTE::set_mac0_flag(int64_t value) {
    if (value < -0x80000000LL) {
        FLAG |= 0x8000;
    }
    else if (value > 0x7FFFFFFF) {
        FLAG |= 0x10000;
    }
    return (int)value;
}

uint16_t GTE::set_sz3_flag(int64_t value) {
    if (value < 0) {
        FLAG |= 0x40000;
        return 0;
    }

    if (value > 0xFFFF) {
        FLAG |= 0x40000;
        return 0xFFFF;
    }

    return (uint16_t)value;
}

int16_t GTE::set_sxy_flag(int i, int32_t value) {
    if (value < -0x400) {
        FLAG |= (uint32_t)(0x4000 >> (i - 1));
        return -0x400;
    }

    if (value > 0x3FF) {
        FLAG |= (uint32_t)(0x4000 >> (i - 1));
        return 0x3FF;
    }

    return (short)value;
}

int16_t GTE::set_ir0_flag(int64_t value) {
    if (value < 0) {
        FLAG |= 0x1000;
        return 0;
    }

    if (value > 0x1000) {
        FLAG |= 0x1000;
        return 0x1000;
    }

    return (short)value;
}

int16_t GTE::set_ir_flag(int i, int32_t value, bool lm) {
    if (lm && value < 0) {
        FLAG |= (uint32_t)(0x1000000 >> (i - 1));
        return 0;
    }

    if (!lm && (value < -0x8000)) {
        FLAG |= (uint32_t)(0x1000000 >> (i - 1));
        return -0x8000;
    }

    if (value > 0x7FFF) {
        FLAG |= (uint32_t)(0x1000000 >> (i - 1));
        return 0x7FFF;
    }

    return (short)value;
}

uint8_t GTE::set_rgb(int i, int value) {
    if (value < 0) {
        FLAG |= (uint32_t)0x200000 >> (i - 1);
        return 0;
    }

    if (value > 0xFF) {
        FLAG |= (uint32_t)0x200000 >> (i - 1);
        return 0xFF;
    }

    return (uint8_t)value;
}

Matrix GTE::get_mvmva_matrix(uint32_t command) {
    GTECommand com;
    com.raw = command;
    switch (com.mvmva_mul_matrix) {
        case 0: return RT;
        case 1: return LM;
        case 2: return LRGB;
        default: exit(0);
    }
}

Vec3i16 GTE::get_mvmva_vector(uint32_t command) {
    GTECommand com;
    com.raw = command;
    Vec3i16 value;
    switch (com.mvmva_mul_vec) {
        case 0: value = V[0]; break;
        case 1: value = V[1]; break;
        case 2: value = V[2]; break;
        case 3: value.vector = { IR[1], IR[2], IR[3] }; break;
    }

    return value;
}

std::tuple<int, int, int> GTE::get_mvmva_translation(uint32_t command) {
    GTECommand com;
    com.raw = command;
    switch (com.mvmva_tr_vec) {
        case 0: return std::make_tuple(TRX, TRY, TRZ);
        case 1: return std::make_tuple(RBK, GBK, BBK);
        case 2: return std::make_tuple(RFC, GFC, BFC);
        case 3: return std::make_tuple(0, 0, 0);
        default: exit(0);
    }
}

void GTE::interpolate(int32_t mac1, int32_t mac2, int32_t mac3, uint32_t command) {
    GTECommand com;
    com.raw = command;

    MAC1 = (int)(set_mac_flag(1, ((int64_t)RFC << 12) - mac1) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, ((int64_t)GFC << 12) - mac2) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, ((int64_t)BFC << 12) - mac3) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, false);
    IR[2] = set_ir_flag(2, MAC2, false);
    IR[3] = set_ir_flag(3, MAC3, false);

    MAC1 = (int)(set_mac_flag(1, ((int64_t)IR[1] * IR[0]) + mac1) >> com.sf * 12);
    MAC2 = (int)(set_mac_flag(2, ((int64_t)IR[2] * IR[0]) + mac2) >> com.sf * 12);
    MAC3 = (int)(set_mac_flag(3, ((int64_t)IR[3] * IR[0]) + mac3) >> com.sf * 12);

    IR[1] = set_ir_flag(1, MAC1, com.lm);
    IR[2] = set_ir_flag(2, MAC2, com.lm);
    IR[3] = set_ir_flag(3, MAC3, com.lm);
}