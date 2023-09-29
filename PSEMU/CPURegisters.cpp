/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "CPURegisters.h"

void CPURegisters::setC0Register(Coprocessor0::C0Register rega, uint32_t value) {
    // Set the value of the specified c0 register
    switch (rega) {
        case Coprocessor0::STATUS:
            reg[rega] = value;
            break;
        case Coprocessor0::CAUSE:
            reg[rega] = value;
            break;
        case Coprocessor0::BAD_ADDRESS:
            reg[rega] = value;
            break;
        case Coprocessor0::EPC:
            reg[rega] = value;
            break;
        default:
            break;
    }
}

uint32_t CPURegisters::getC0Register(Coprocessor0::C0Register rega) {
    // Returns the value of the specified c0 register
    return reg[rega];
}