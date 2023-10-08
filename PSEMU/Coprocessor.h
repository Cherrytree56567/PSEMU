/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <fstream>
#include <bitset>

class Coprocessor0 {
public:
    uint32_t readRegister(uint32_t rega) override {
        return reg[rega];
    }

    void writeRegister(uint32_t reg, uint32_t value) override {
        reg[reg] = value;
    }

private:
    uint32_t reg[64];
};
