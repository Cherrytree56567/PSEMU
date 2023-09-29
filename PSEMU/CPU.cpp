/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "CPU.h"
#pragma warning(disable : 4996)

// First Add rs and rt then store in rd (register)

void CPU::op_add(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (rt > 32) {
        Logging console;
        // Overflow Exception: RT is greater than 32.
        console.err(51);
    } else {
        std::cout << "ADDING: RESULT = " << registers.reg[rs] + registers.reg[rt] << ", RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;

        registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
    }
}

// First take the data from RT (Register) and store it in IMM + RS (memory)

void CPU::op_storebyte(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t rsValue = registers.reg[rs];
    uint8_t valueToStore = rsValue & 0xFF;    // Extract the least significant byte

    // Calculate the effective memory address
    uint32_t effectiveAddress = rsValue + imm;

    // Store the value in memory
    memory[effectiveAddress] = valueToStore;

    std::cout << "STOREBYTE: Value = " << std::to_string(valueToStore) << ", RS = " << std::to_string(rs) << ", Immediate = " << std::to_string(imm) << std::endl;
}

// lui is used to load a value into a register. example: "lui $t0, 0x1234"

void CPU::op_lui(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    registers.reg[rs] = imm;

    std::cout << "Loading Value: RS = " << std::to_string(rs) << ", IMM = " << std::to_string(imm) << std::endl;
} 

// The op_addi function adds imm and rs, and stores the result in rt.

void CPU::op_addi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    
    if (rt > 32) {
        Logging console;
        console.err(51);
    } else {
        registers.reg[rt] = imm + registers.reg[rs];

        std::cout << "Adding Immediate Value: RS = " << std::to_string(rs) << ", IMM = " << std::to_string(imm) << ", RT = " << std::to_string(rt) << std::endl;
    }
}

// op_addu adds values in two registers and stores in another. No overflow exception raised.

void CPU::op_addu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (rt > 32) {
        Logging console;
        console.log("INFO: Overflow Exception: RT is greater than 32. THIS iS NOT CONCERNING BECAUSE WE ARE USING ADDU. RT = " + std::to_string(rt));
    } else {
        std::cout << "ADDING Unsigned: RESULT = " << registers.reg[rs] + registers.reg[rt] << ", RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;

        registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
    }
}

// op_addiu adds imm and rs, and stores the result in rt. No overflow exception raised.

void CPU::op_addiu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (rt > 32) {
        Logging console;
        console.log("INFO: Overflow Exception: RT is greater than 32. THIS iS NOT CONCERNING BECAUSE WE ARE USING ADDIU. RT = " + std::to_string(rt));
    } else {
        registers.reg[rt] = imm + registers.reg[rs];

        std::cout << "Adding Immediate Value: RS = " << std::to_string(rs) << ", IMM = " << std::to_string(imm) << ", RT = " << std::to_string(rt) << std::endl;
    }
}

// op_and compares rs and rd and stores 1 in rd if they are equal, otherwise 0.

void CPU::op_and(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (registers.reg[rs] == registers.reg[rt]){
        registers.reg[rd] = 1;
    } else {
        registers.reg[rd] = 0;
    }

    std::cout << "Boolean: RESULT = " << std::to_string(registers.reg[rd]) << ", RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;
}

// op_and compares rs and imm and stores 1 in rt if they are equal, otherwise 0.

void CPU::op_andi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] == imm){
        registers.reg[rt] = 1;
    } else {
        registers.reg[rt] = 0;
    }

    std::cout << "Boolean: RESULT = " << std::to_string(registers.reg[rt]) << ", RS = " << std::to_string(rs) << ", IMM = " << std::to_string(imm) << std::endl;

}

// Compare rs and rt, if they are equal, jump to the target address (imm)

void CPU::op_beq(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] == registers.reg[rt]) {
        registers.pc += imm; // Branch to the target address if the values are equal
    }

    std::cout << "BEQ: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

// Branches to imm if the value in rs is less than or equal to 0

void CPU::op_blez(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] <= 0) {
        registers.pc += imm;
    }

    std::cout << "BLEZ: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

// Branches to imm if the values in rs and rt are not equal

void CPU::op_bne(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] != registers.reg[rt]) {
        registers.pc += imm;
    }

    std::cout << "BNE: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

// Branches to imm if the value in rs is greater than 0

void CPU::op_bgtz(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] > 0) {
        registers.pc += imm; 
    }

    std::cout << "BGTZ: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

void CPU::op_div(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint32_t dividend = static_cast<uint32_t>(registers.reg[rs]); // Convert the register value to an unsigned integer
    uint32_t divisor = static_cast<uint32_t>(registers.reg[rt]); // Convert the register value to an unsigned integer

    if (divisor != 0) {
        registers.lo = dividend / divisor; // Store the quotient in LO
        registers.hi = dividend % divisor; // Store the remainder in HI
    }

    std::cout << "DIVU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;
}

void CPU::op_divu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint32_t dividend = static_cast<uint32_t>(registers.reg[rs]); // Convert the register value to an unsigned integer
    uint32_t divisor = static_cast<uint32_t>(registers.reg[rt]); // Convert the register value to an unsigned integer

    if (divisor != 0) {
        registers.lo = dividend / divisor; // Store the quotient in LO
        registers.hi = dividend % divisor; // Store the remainder in HI
    }

    std::cout << "DIVU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;
}

void CPU::op_j(uint32_t instruction) {
    uint32_t target = (instruction & 0x03FFFFFF) << 2; // Extract the target address and shift it left by 2 bits
    uint32_t pc_upper = registers.pc & 0xF0000000; // Extract the upper 4 bits of the current PC value
    registers.pc = pc_upper | target; // Set the PC to the target address

    std::cout << "J: TARGET = " << std::to_string(target) << std::endl;
}

void CPU::op_jal(uint32_t instruction) {
    uint32_t target = (instruction & 0x03FFFFFF) << 2; // Extract the target address and shift it left by 2 bits
    uint32_t pc_plus_4 = registers.pc + 4; // Calculate the address of the instruction after the JAL
    registers.reg[31] = pc_plus_4; // Store the return address in RA
    uint32_t pc_upper = registers.pc & 0xF0000000; // Extract the upper 4 bits of the current PC value
    registers.pc = pc_upper | target; // Set the PC to the target address

    std::cout << "JAL: TARGET = " << std::to_string(target) << ", RA = " << std::to_string(registers.reg[31]) << std::endl;
}

void CPU::op_jalr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint32_t target = registers.reg[rs]; // Get the target address from the specified register
    uint32_t pc_plus_4 = registers.pc + 4; // Calculate the address of the instruction after the JALR
    registers.reg[31] = pc_plus_4; // Store the return address in RA
    registers.pc = target; // Set the PC to the target address

    std::cout << "JALR: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", TARGET = " << std::to_string(target) << ", RA = " << std::to_string(registers.reg[31]) << std::endl;
}

void CPU::op_jr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint32_t target = registers.reg[rs]; // Get the target address from the specified register
    registers.pc = target; // Set the PC to the target address

    std::cout << "JR: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", TARGET = " << std::to_string(target) << std::endl;
}

void CPU::op_lbu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF; // Extract the immediate value

    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address
    uint8_t value = memory.readByte(address); // Read the byte from memory
    registers.reg[rt] = static_cast<uint32_t>(value); // Store the value in the specified register

    std::cout << "LBU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(value) << std::endl;
}

void CPU::op_lhu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address
    uint32_t value = memory.readWord(address); // Read the word from memory
    uint16_t halfword = static_cast<uint16_t>((value >> ((address & 2) << 3)) & 0xFFFF); // Extract the halfword from the word
    registers.reg[rt] = static_cast<uint32_t>(halfword); // Store the value in the specified register

    std::cout << "LHU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(halfword) << std::endl;
}

void CPU::op_lw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address
    uint32_t value = memory.readWord(address); // Read the word from memory
    registers.reg[rt] = value; // Store the value in the specified register

    std::cout << "LW: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(value) << std::endl;
}

void CPU::op_mfhi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.hi; // Move the value in the HI register to the specified register

    std::cout << "MFHI: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", VALUE = " << std::to_string(registers.hi) << std::endl;
}

void CPU::op_mthi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.hi = registers.reg[rs]; // Move the value in the specified register to the HI register

    std::cout << "MTHI: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", VALUE = " << std::to_string(registers.hi) << std::endl;
}

void CPU::op_mflo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.lo; // Move the value in the LO register to the specified register

    std::cout << "MFLO: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", VALUE = " << std::to_string(registers.lo) << std::endl;
}

void CPU::op_mtlo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.lo = registers.reg[rs]; // Move the value in the specified register to the LO register

    std::cout << "MTLO: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", VALUE = " << std::to_string(registers.lo) << std::endl;
}

void CPU::op_mfc0(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint32_t value = coprocessor0.readRegister(rs); // Read the value from the specified coprocessor 0 register
    registers.reg[rt] = value; // Move the value to the specified general-purpose register

    std::cout << "MFC0: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", VALUE = " << std::to_string(value) << std::endl;
}

void CPU::op_mult(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    int64_t result = static_cast<int64_t>(static_cast<int32_t>(registers.reg[rs])) * static_cast<int64_t>(static_cast<int32_t>(registers.reg[rt])); // Perform the multiplication as a signed 64-bit integer
    registers.lo = static_cast<uint32_t>(result & 0xFFFFFFFF); // Store the lower 32 bits of the result in the LO register
    registers.hi = static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF); // Store the upper 32 bits of the result in the HI register

    std::cout << "MULT: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(result) << std::endl;
}

void CPU::op_multu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    uint64_t result = static_cast<uint64_t>(registers.reg[rs]) * static_cast<uint64_t>(registers.reg[rt]); // Perform the multiplication as an unsigned 64-bit integer
    registers.lo = static_cast<uint32_t>(result & 0xFFFFFFFF); // Store the lower 32 bits of the result in the LO register
    registers.hi = static_cast<uint32_t>((result >> 32) & 0xFFFFFFFF); // Store the upper 32 bits of the result in the HI register

    std::cout << "MULTU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(result) << std::endl;
}

void CPU::op_nor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = ~(registers.reg[rs] | registers.reg[rt]); // Perform the bitwise NOR operation and store the result in the specified register

    std::cout << "NOR: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_xor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] ^ registers.reg[rt]; // Perform the bitwise XOR operation and store the result in the specified register

    std::cout << "XOR: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_or(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] | registers.reg[rt]; // Perform the bitwise OR operation and store the result in the specified register

    std::cout << "OR: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_ori(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    registers.reg[rt] = registers.reg[rs] | imm; // Perform the bitwise OR operation between the value in the specified register and the immediate value, and store the result in the specified register

    std::cout << "ORI: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_lb(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t address = registers.reg[rs] + static_cast<int16_t>(imm); // Calculate the memory address
    uint8_t value = memory.readByte(address); // Read the byte from memory
    int8_t signedValue = static_cast<int8_t>(value); // Sign-extend the byte to a 32-bit integer
    registers.reg[rt] = static_cast<uint32_t>(signedValue); // Store the signed value in the specified register

    std::cout << "LB: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_sh(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t address = registers.reg[rs] + static_cast<int16_t>(imm); // Calculate the memory address
    uint16_t value = static_cast<uint16_t>(registers.reg[rt] & 0xFFFF); // Extract the least significant 2 bytes of the value in the specified register
    memory.writeHalfword(address, value); // Write the halfword to memory

    std::cout << "SH: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(value) << std::endl;
}

void CPU::op_slt(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (static_cast<int32_t>(registers.reg[rs]) < static_cast<int32_t>(registers.reg[rt])) { // Compare the values in the specified registers
        registers.reg[rd] = 1; // If rs < rt, set rd to 1
    } else {
        registers.reg[rd] = 0; // Otherwise, set rd to 0
    }

    std::cout << "SLT: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_slti(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    int16_t imm = static_cast<int16_t>(instruction & 0xFFFF); // Extract the signed immediate value

    if (static_cast<int32_t>(registers.reg[rs]) < imm) { // Compare the value in the specified register with the immediate value
        registers.reg[rt] = 1; // If rs < imm, set rt to 1
    } else {
        registers.reg[rt] = 0; // Otherwise, set rt to 0
    }

    std::cout << "SLTI: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_sltiu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    if (registers.reg[rs] < imm) { // Compare the value in the specified register with the immediate value
        registers.reg[rt] = 1; // If rs < imm, set rt to 1
    } else {
        registers.reg[rt] = 0; // Otherwise, set rt to 0
    }

    std::cout << "SLTIU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_sltu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (registers.reg[rs] < registers.reg[rt]) { // Compare the values in the specified registers
        registers.reg[rd] = 1; // If rs < rt, set rd to 1
    } else {
        registers.reg[rd] = 0; // Otherwise, set rd to 0
    }

    std::cout << "SLTU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_sll(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6

    registers.reg[rt] = registers.reg[rs] << sa; // Shift the bits in the specified register to the left by the specified amount, and store the result in the specified register

    std::cout << "SLL: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", SA = " << std::to_string(sa) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_srl(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6

    registers.reg[rt] = static_cast<uint32_t>(registers.reg[rs]) >> sa; // Shift the bits in the specified register to the right by the specified amount, and store the result in the specified register

    std::cout << "SRL: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", SA = " << std::to_string(sa) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_sra(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6

    registers.reg[rt] = static_cast<int32_t>(registers.reg[rs]) >> sa; // Shift the bits in the specified register to the right by the specified amount, filling the vacated bits with the sign bit, and store the result in the specified register

    std::cout << "SRA: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", SA = " << std::to_string(sa) << ", RESULT = " << std::to_string(registers.reg[rt]) << std::endl;
}

void CPU::op_sub(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    int32_t result = static_cast<int32_t>(registers.reg[rs]) - static_cast<int32_t>(registers.reg[rt]); // Subtract the value in rt from the value in rs, and store the result in a signed 32-bit integer

    registers.reg[rd] = static_cast<uint32_t>(result); // Store the result in the specified register

    std::cout << "SUB: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_subu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] - registers.reg[rt]; // Subtract the value in rt from the value in rs, and store the result in the specified register

    std::cout << "SUBU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << ", RESULT = " << std::to_string(registers.reg[rd]) << std::endl;
}

void CPU::op_sw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address by adding the value in rs to the immediate value

    memory.writeWord(address, registers.reg[rt]); // Write the value in rt to memory at the calculated address

    std::cout << "SW: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << std::endl;
}

void CPU::loadBIOS(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file");
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the number of 32-bit chunks
    size_t numChunks = (fileSize + sizeof(uint32_t) - 1) / sizeof(uint32_t);

    Logging console;
    console.log("NumChunks " + std::to_string(numChunks));

    // Allocate memory for the array
    uint32_t* aBiosCode = (uint32_t*)malloc(numChunks * sizeof(uint32_t));
    if (!aBiosCode) {
        console.err(52);
        fclose(file);
    }

    // Read and separate the content into 32-bit chunks
    size_t bytesRead = fread(aBiosCode, sizeof(uint32_t), numChunks, file);
    if (bytesRead < numChunks) {
        console.err(53);
        free(aBiosCode);
        fclose(file);
    }

    fclose(file);

    BiosCode = aBiosCode;
    this->numInstructions = numChunks;
}

void CPU::loadBiosCode(uint32_t* binaryCode, size_t numI) {
    this->numInstructions = numI;
    std::cout << "dff" << this->numInstructions;
    BiosCode = binaryCode;
}

void CPU::loadInstructions() {
    for (size_t i = 0; i < numInstructions; ++i) {
        memory.writeWord(i * 4, BiosCode[i]); // Each instruction is 4 bytes
    }
}

void CPU::run() {
    Logging console;
    registers.pc = 0; // Start from the beginning of memory

    while (registers.pc < numInstructions * 4) {
        uint32_t instruction = memory.readWord(registers.pc);
        uint8_t opcode = (instruction >> 26) & 0x3F; // OPCODE = First 6 bits
        uint8_t funct = instruction & 0x3F; // FUNCTION_CODE = Last 6 bits

        switch (opcode) {
        case 0b000000: // R-type instructions
            switch (funct) {
            case 0b100000:
                // Add
                op_add(instruction);
                console.log("CPU INSTRUCTION :: ADD");
                break;

            case 0b100001:
                // Addu
                op_addu(instruction);
                console.log("CPU INSTRUCTION :: ADDU");
                break;

            case 0b100100:
                // and
                op_and(instruction);
                console.log("CPU INSTRUCTION :: AND");
                break;

            case 0b011010:
                // div
                op_div(instruction);
                console.log("CPU INSTRUCTION :: DIV");
                break;

            case 0b011011:
                // divu
                op_divu(instruction);
                console.log("CPU INSTRUCTION :: DIVU");
                break;

            case 0b001001:
                // jalr
                op_jalr(instruction);
                console.log("CPU INSTRUCTION :: JALR");
                break;

            case 0b001000:
                // jr
                op_jr(instruction);
                console.log("CPU INSTRUCTION :: JR");
                break;

            case 0b010000:
                // mfhi
                op_mfhi(instruction);
                console.log("CPU INSTRUCTION :: MFHI");
                break;

            case 0b010001:
                // mthi
                op_mthi(instruction);
                console.log("CPU INSTRUCTION :: MTHI");
                break;

            case 0b010010:
                // mflo
                op_mflo(instruction);
                console.log("CPU INSTRUCTION :: MFLO");
                break;

            case 0b010011:
                // mtlo
                op_mtlo(instruction);
                console.log("CPU INSTRUCTION :: MTLO");
                break;

            case 0b011000:
                // mult
                op_mult(instruction);
                console.log("CPU INSTRUCTION :: MULT");
                break;

            case 0b011001:
                // multu
                op_multu(instruction);
                console.log("CPU INSTRUCTION :: MULTU");
                break;

            case 0b100111:
                // nor
                op_nor(instruction);
                console.log("CPU INSTRUCTION :: NOR");
                break;

            case 0b100110:
                // xor
                op_xor(instruction);
                console.log("CPU INSTRUCTION :: XOR");
                break;

            case 0b100101:
                // or
                op_or(instruction);
                console.log("CPU INSTRUCTION :: OR");
                break;

            case 0b101010:
                // slt
                op_slt(instruction);
                console.log("CPU INSTRUCTION :: SLT");
                break;

            case 0b101011:
                // sltu
                op_sltu(instruction);
                console.log("CPU INSTRUCTION :: SLTU");
                break;

            case 0b000000:
                // sll
                op_sll(instruction);
                console.log("CPU INSTRUCTION :: SLL");
                break;

            case 0b000010:
                // srl
                op_srl(instruction);
                console.log("CPU INSTRUCTION :: SRL");
                break;

            case 0b000011:
                // sra
                op_sra(instruction);
                console.log("CPU INSTRUCTION :: SRA");
                break;

            case 0b100010:
                // sub
                op_sub(instruction);
                console.log("CPU INSTRUCTION :: SUB");
                break;

            case 0b100011:
                // subu
                op_subu(instruction);
                console.log("CPU INSTRUCTION :: SUBU");
                break;

            default:
                // Handle invalid funct code
                Logging console;
                console.warn("Invalid Function code: " + std::to_string(funct));
                break;
            }
            break;

        // J Type Instructions
        case 0b000010:
            // j
            op_j(instruction);
            console.log("CPU INSTRUCTION :: J");
            break;

        case 0b000011:
            // jal
            op_jal(instruction);
            console.log("CPU INSTRUCTION :: JAL");
            break;

        // I Type Instructions
        case 0b001000:
            // addi
            op_addi(instruction);
            console.log("CPU INSTRUCTION :: ADDI");
            break;

        case 0b001001:
            // addiu
            op_addiu(instruction);
            console.log("CPU INSTRUCTION :: ADDIU");
            break;

        case 0b001100:
            // andi
            op_andi(instruction);
            console.log("CPU INSTRUCTION :: ANDI");
            break;

        case 0b000100:
            // beq
            op_beq(instruction);
            console.log("CPU INSTRUCTION :: BEQ");
            break;

        case 0b000110:
            // blez
            op_blez(instruction);
            console.log("CPU INSTRUCTION :: BLEZ");
            break;

        case 0b000101:
            // bne
            op_bne(instruction);
            console.log("CPU INSTRUCTION :: BNE");
            break;

        case 0b000111:
            // bgtz
            op_bgtz(instruction);
            console.log("CPU INSTRUCTION :: BGTZ");
            break;

        case 0b100000:
            // lb
            op_lb(instruction);
            console.log("CPU INSTRUCTION :: LB");
            break;

        case 0b100100:
            // lbu
            op_lbu(instruction);
            console.log("CPU INSTRUCTION :: LBU");
            break;

        case 0b100101:
            // lhu
            op_lhu(instruction);
            console.log("CPU INSTRUCTION :: LHU");
            break;

        case 0b001111:
            // lui
            op_lui(instruction);
            console.log("CPU INSTRUCTION :: LUI");
            break;

        case 0b100011:
            // lw
            op_lw(instruction);
            console.log("CPU INSTRUCTION :: LW");
            break;

        case 0b010000:
            // mfc0
            op_mfc0(instruction);
            console.log("CPU INSTRUCTION :: MFC0");
            break;

        case 0b001101:
            // ori
            op_ori(instruction);
            console.log("CPU INSTRUCTION :: ORI");
            break;

        case 0b101000:
            // sb
            op_storebyte(instruction);
            console.log("CPU INSTRUCTION :: SB");
            break;

        case 0b101001:
            // sh
            op_sh(instruction);
            console.log("CPU INSTRUCTION :: SH");
            break;

        case 0b001010:
            // slti
            op_slti(instruction);
            console.log("CPU INSTRUCTION :: SLTI");
            break;

        case 0b001011:
            // sltiu
            op_sltiu(instruction);
            console.log("CPU INSTRUCTION :: SLTIU");
            break;

        case 0b101011:
            // sw
            op_sw(instruction);
            console.log("CPU INSTRUCTION :: SW");
            break;

        default:
            Logging console;
            console.warn("Invalid Opcode: " + std::bitset<6>(opcode).to_string());
            break;
        }

        // Program Counter
        registers.pc += 4; // 4 Bytes = 32 Bits

        // Interrupts
        if (checkForInterrupts()) {
            handleInterrupts();
        }
    }
    for (int i = 0; i < 32; ++i) {
        console.log("Register " + std::to_string(i) + ": " + std::to_string(registers.reg[i]));
    }
    /*for (int i = 0; i < memory.memory.size(); ++i) {
        console.log("MEMORY " + std::to_string(i) + ": " + std::bitset<32>(memory.memory[i]).to_string());
    }*/
}
