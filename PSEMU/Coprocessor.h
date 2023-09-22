#pragma once
#include <fstream>
#include <bitset>

class Coprocessor {
public:
    virtual uint32_t readRegister(uint8_t reg) = 0;
    virtual void writeRegister(uint8_t reg, uint32_t value) = 0;
};

class Coprocessor0 : public Coprocessor {
public:
    enum C0Register {
        STATUS = 12,
        CAUSE = 13,
        BAD_ADDRESS = 14,
        EPC = 15
    };

    uint32_t readRegister(uint8_t reg) override {
        switch (reg) {
        case 12:
            return status;
        case 13:
            return cause;
        case 14:
            return bad_address;
        case 15:
            return epc;
        default:
            return 0;
        }
    }

    void writeRegister(uint8_t reg, uint32_t value) override {
        switch (reg) {
        case 12:
            status = value;
            break;
        case 13:
            cause = value;
            break;
        case 14:
            bad_address = value;
            break;
        case 15:
            epc = value;
            break;
        }
    }

private:
    uint32_t status = 0;
    uint32_t cause = 0;
    uint32_t bad_address = 0;
    uint32_t epc = 0;
};
