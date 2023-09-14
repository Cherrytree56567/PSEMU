#include <fstream>
#include <bitset>

class Coprocessor {
public:
    virtual uint32_t readRegister(uint8_t reg) = 0;
    virtual void writeRegister(uint8_t reg, uint32_t value) = 0;
};

class Coprocessor0 : public Coprocessor {
public:
    uint32_t readRegister(uint8_t reg) override {
        switch (reg) {
            case 12:
                return status;
            default:
                return 0;
        }
    }

    void writeRegister(uint8_t reg, uint32_t value) override {
        switch (reg) {
            case 12:
                status = value;
                break;
        }
    }

private:
    uint32_t status = 0;
};