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
    switch (command) {
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
    // Empty Implementation
}

void GTE::op_nclip(uint32_t command) {
    // Empty Implementation
}

void GTE::op_op(uint32_t command) {
    // Empty Implementation
}

void GTE::op_mvmva(uint32_t command) {
    // Empty Implementation
}

void GTE::op_dpcs(uint32_t command) {
    // Empty Implementation
}

void GTE::op_intpl(uint32_t command) {
    // Empty Implementation
}

void GTE::op_cc(uint32_t command) {
    // Empty Implementation
}

void GTE::op_rtpt(uint32_t command) {
    // Empty Implementation
}

void GTE::op_ncds(uint32_t command) {
    // Empty Implementation
}

void GTE::op_avsz3(uint32_t command) {
    // Empty Implementation
}

void GTE::op_ncct(uint32_t command) {
    // Empty Implementation
}

void GTE::op_gpf(uint32_t command) {
    // Empty Implementation
}

void GTE::op_sqr(uint32_t command) {
    // Empty Implementation
}

void GTE::op_avsz4(uint32_t command) {
    // Empty Implementation
}

void GTE::op_nct(uint32_t command) {
    // Empty Implementation
}

void GTE::op_ncdt(uint32_t command) {
    // Empty Implementation
}

void GTE::op_nccs(uint32_t command) {
    // Empty Implementation
}

void GTE::op_ncs(uint32_t command) {
    // Empty Implementation
}
