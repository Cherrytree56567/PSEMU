/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <cstdint>

class GTE {
public:
    GTE();

    // GTE Data Registers
    // I think these like parameters, but like 1 variable with different types
    int16_t VXY0[3];  // Vector 0 (X, Y, Z)
    int16_t VXY1[3];  // Vector 1 (X, Y, Z)
    int16_t VXY2[3];  // Vector 2 (X, Y, Z)
    uint8_t RGBC[4];  // Color/code value
    uint16_t OTZ;     // Average Z value
    int16_t IR0;      // 16-bit Accumulator (Interpolate)
    int16_t IR1[3];   // 16-bit Accumulator (Vector)
    int16_t SXY0[2];  // Screen XY-coordinate FIFO (3 stages)
    int16_t SXY1[2];
    int16_t SXY2[2];
    uint16_t SZ[4];   // Screen Z-coordinate FIFO (4 stages)
    uint8_t RGB[3][3]; // Color CRGB-code/color FIFO (3 stages)
    int32_t MAC0;      // 32-bit Math Accumulators (Value)
    int32_t MAC1[3];   // 32-bit Math Accumulators (Vector)
    uint16_t IRGB[2];  // Convert RGB Color (48-bit vs 15-bit)
    int32_t LZCS;      // Count Leading Zeros (Sign Bits)
    int32_t LZCR;      // Count Leading Ones (Sign Bits)

    // GTE Control Registers
    // I think these are also parameters
    int16_t RT[9];     // Rotation matrix (3x3)
    int32_t TR[3];     // Translation vector (X, Y, Z)
    int16_t L[9];      // Light source matrix (3x3)
    int32_t BKGRGB[3]; // Background color (R, G, B)
    int16_t LR[9];     // Light color matrix source (3x3)
    int32_t FARGB[3];  // Far color (R, G, B)
    int32_t ScreenOffset[2]; // Screen offset (X, Y)
    uint16_t ProjectionPlaneDistance; // Projection plane distance
    int16_t DepthQueuingParamA; // Depth queuing parameter A
    int32_t DepthQueuingParamB; // Depth queuing parameter B
    int16_t AverageZScaleFactors[2]; // Average Z scale factors
    uint32_t Flags;   // Calculation error flags

    // GTE commands here

    // Execute GTE command
    void ExecuteCommand(uint32_t command);
};
