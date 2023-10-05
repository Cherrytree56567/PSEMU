/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include "CPU.h"
#pragma warning(disable : 4996)


/*
 * Interesting code from GPUCode/PSXEmu.
    void CPU::op_cop2()
    {
        switch (instr.rs() & 0x10) {
        case 0x00:
            switch (instr.rs()) {
            case 0b00000: op_mfc2(); break;
            case 0b00010: op_cfc2(); break;
            case 0b00100: op_mtc2(); break;
            case 0b00110: op_ctc2(); break;
            default: exception(ExceptionType::IllegalInstr, instr.id()); break;
            }
            break;
        case 0x10:
            gte.execute(instr);
            break;
        }
    }

    void CPU::op_cop0()
    {
        switch (instr.rs()) {
        case 0b00000: op_mfc0(); break;
        case 0b00100: op_mtc0(); break;
        case 0b10000: op_rfe(); break;
        default: exception(ExceptionType::IllegalInstr, instr.id()); break;
        }
    }
 */

/*
 * /Add Bcond   0b000001
 * /Add Cop0    0b010000
 * /Add Cop2    0b010010
 * /Add xori    0b001110
 * /Add swr     0b101110
 * /Add swl     0b101010
 * /Add lwl     0b100010
 * /Add lwr     0b100110
 * /Add lwc2    0b110010
 * /Add swc2    0b111010
 * /Add syscall 0b001100
 * /Add sllv    0b000100
 * /Add srav    0b000111
 * /Add srlv    0b000110
 * /Add break   0b001101
 */

using uint = std::uint32_t;
using ulong = std::uint64_t;
// First Add rs and rt then store in rd (register)

void CPU::op_add(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11
    rs = registers.reg[rs];
    rt = registers.reg[rt];

    if (rt > 32) {
        Logging console;
        // Overflow Exception: RT is greater than 32.
        console.err(51);
    } else {
        registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
    }
}

// First take the data from RT (Register) and store it in IMM + RS (memory)

void CPU::op_storebyte(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    // Store the value in memory
    memory[registers.reg[rs] + imm_s] = registers.reg[rt];
}

// lui is used to load a value into a register. example: "lui $t0, 0x1234"

void CPU::op_lui(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    registers.reg[rt] = imm << 16;
} 

// The op_addi function adds imm and rs, and stores the result in rt.

void CPU::op_addi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    if (rt > 32) {
        Logging console;
        console.err(51);
    } else {
        registers.reg[rt] = imm_s + rs;
    }
}

// op_addu adds values in two registers and stores in another. No overflow exception raised.

void CPU::op_addu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
}

// op_addiu adds imm and rs, and stores the result in rt. No overflow exception raised.

void CPU::op_addiu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    registers.reg[rt] = imm_s + registers.reg[rs];
}

// op_and compares rs and rd and stores 1 in rd if they are equal, otherwise 0.

void CPU::op_and(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] & registers.reg[rt];
}
//g

// op_and compares rs and imm and stores 1 in rt if they are equal, otherwise 0.

void CPU::op_andi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    registers.reg[rt] = registers.reg[rs] & registers.reg[imm];
}

//

// Compare rs and rt, if they are equal, jump to the target address (imm)

void CPU::op_beq(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (unsigned int)(int16_t)imm;      // Extract the immediate value

    if (registers.reg[rs] == registers.reg[rt]) {
        registers.next_pc = registers.pc + (imm_s << 2); // Branch to the target address if the values are equal
    }
}

// Branches to imm if the value in rs is less than or equal to 0

void CPU::op_blez(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    if ((int)registers.reg[rs] <= 0) {
        registers.next_pc = registers.pc + (imm_s << 2);
    }

    std::cout << "BLEZ: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

// Branches to imm if the values in rs and rt are not equal

void CPU::op_bne(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    if (registers.reg[rs] != registers.reg[rt]) {
        registers.next_pc = registers.pc + (imm_s << 2);
    }

    std::cout << "BNE: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

// Branches to imm if the value in rs is greater than 0

void CPU::op_bgtz(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;      // Extract the immediate value

    if ((int)registers.reg[rs] > 0) {
        registers.next_pc = registers.pc + (imm_s << 2);
    }

    std::cout << "BGTZ: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << std::endl;
}

void CPU::op_div(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if ((int)rt == 0){
        registers.hi = (uint)rs;
        if ((int)rs >= 0) {
            registers.lo = 0xFFFFFFFF;
        }
        else {
            registers.lo = 1;
        }
    } else if ((uint)rs == 0x80000000 && (int)rt == -1) {
        registers.hi = 0;
        registers.lo = 0x80000000;
    } else {
        registers.hi = (uint)((int)rs % (int)rt);
        registers.lo = (uint)((int)rs / (int)rt);
    }
}

//

void CPU::op_divu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11
    
    uint n = registers.reg[rs];
    uint d = registers.reg[rt];

    if (d == 0) {
      registers.hi = n;
      registers.lo = 0xFFFFFFFF;
    } else {
      registers.hi = n % d;
      registers.lo = n / d;
    }
}

void CPU::op_j(uint32_t instruction) {
    uint addr = instruction & 0x3FFFFFF;
    registers.next_pc = (registers.next_pc & 0xF0000000) | (addr << 2);
}

void CPU::op_jal(uint32_t instruction) {
    uint addr = instruction & 0x3FFFFFF;
    registers.reg[31] = registers.next_pc;
    op_j(instruction);
}

//

void CPU::op_jalr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11
    
    registers.reg[rd] = registers.next_pc;
    op_jr(instruction);
}

//

void CPU::op_jr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.next_pc = registers.reg[rs];
}

//

void CPU::op_lbu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF; // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;

    uint8_t value = memory.readByte(registers.reg[rs] + imm_s; ); // Read the byte from memory
    registers.reg[rt] = static_cast<uint32_t>(value); // Store the value in the specified register

    std::cout << "LBU: RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", IMM = " << std::to_string(imm) << ", ADDRESS = " << std::to_string(address) << ", VALUE = " << std::to_string(value) << std::endl;
}

//

void CPU::op_lhu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;
    
    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address
    uint32_t value = memory.readWord(address); // Read the word from memory
    uint16_t halfword = static_cast<uint16_t>((value >> ((address & 2) << 3)) & 0xFFFF); // Extract the halfword from the word
    registers.reg[rt] = static_cast<uint32_t>(halfword); // Store the value in the specified register

    uint addr = registers.reg[rs] + imm_s;

        if ((addr & 0x1) == 0) {
            uint value = memory.readWord(addr);
            registers.reg[rt] = value;
        }
        else {
            // error
        }
}

//

void CPU::op_lw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;

    uint addr = registers.reg[rs] + imm_s;

    if ((addr & 0x3) == 0) {
        uint value = memory.readWord(addr);
        registers.reg[rt] = value;
    }
}

void CPU::op_mfhi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.hi; // Move the value in the HI register to the specified register
}

//

void CPU::op_mthi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.hi = registers.reg[rs]; // Move the value in the specified register to the HI register
}

void CPU::op_mflo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.lo; // Move the value in the LO register to the specified register
}

void CPU::op_mtlo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.lo = registers.reg[rs]; // Move the value in the specified register to the LO register
}

void CPU::op_mfc0(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (mfc == 3 || mfc >= 5 && mfc <= 9 || mfc >= 11 && mfc <= 15) {
        registers.reg[rt] = coprocessor0.readRegister(mfc);
    }
    else {
        
    }
}

//

void CPU::op_mult(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11
    
    int64_t value = (int64_t)(int)registers.reg[rs] * (int64_t)(int)registers.reg[rt]; //sign extend to pass amidog cpu test

    registers.hi = (uint)(value >> 32);
    registers.lo = (uint)value;
}

//

void CPU::op_multu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    ulong value = (ulong)registers.reg[rs] * (ulong)registers.reg[rt]; //sign extend to pass amidog cpu test

    registers.hi = (uint)(value >> 32);
    registers.lo = (uint)value;
}

//

void CPU::op_nor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = ~(registers.reg[rs] | registers.reg[rt]); // Perform the bitwise NOR operation and store the result in the specified register
}

//

void CPU::op_xor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] ^ registers.reg[rt]; // Perform the bitwise XOR operation and store the result in the specified register
}

void CPU::op_or(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] | registers.reg[rt]; // Perform the bitwise OR operation and store the result in the specified register
}

void CPU::op_ori(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    registers.reg[rt] = registers.reg[rs] | imm; // Perform the bitwise OR operation between the value in the specified register and the immediate value, and store the result in the specified register
}
//
void CPU::op_lb(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint16_t imm_s = (uint)(int16_t)imm;
    
    uint value = (uint)(byte)memory.readByte(registers.reg[rs] + imm_s);
    registers.reg[rt] = value;
}

void CPU::op_sh(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
    uint16_t imm_s = (uint)(int16_t)imm;

    uint addr = registers.reg[rs] + imm_s;

    if ((addr & 0x1) == 0) {
        memory.writeHalfword(addr, (ushort)registers.reg[rt])
    }
    else {
        
    }
}
//11
void CPU::op_slt(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F;
    
    bool condition = (int)registers.reg[rs] < (int)registers.reg[rt];
    registers.reg[rd] = condition ? 1u : 0u;
}

void CPU::op_slti(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    int16_t imm = static_cast<int16_t>(instruction & 0xFFFF); // Extract the signed immediate value

uint16_t imm_s = (uint)(int16_t)imm;
    bool condition = (int)registers.reg[rs] < (int)imm_s;
    registers.reg[rt] = condition ? 1u : 0u;
}

void CPU::op_sltiu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    uint16_t imm_s = (uint)(int16_t)imm;
    bool condition = (int)registers.reg[rs] < (int)imm_s;
    registers.reg[rt] = condition ? 1u : 0u;
}
//
void CPU::op_sltu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    bool condition = (int)registers.reg[rs] < (int)registers.reg[rt];
    registers.reg[rd] = condition ? 1u : 0u;
}

void CPU::op_sll(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6
    uint8_t rd = (instruction >> 11) & 0x1F;

    registers.reg[rd] = registers.reg[rt] << sa; // Shift the bits in the specified register to the left by the specified amount, and store the result in the specified register
}
//
void CPU::op_srl(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6
uint8_t rd = (instruction >> 11) & 0x1F;

    registers.reg[rd] = registers.reg[rt] >> sa;
}

void CPU::op_sra(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t sa = (instruction >> 6) & 0x1F;  // Extract bits 10 to 6
uint8_t rd = (instruction >> 11) & 0x1F;

    registers.reg[rd] = registers.reg[rt] >> sa;
    
}
//
void CPU::op_sub(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] - registers.reg[rt];
}
//
void CPU::op_subu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    registers.reg[rd] = registers.reg[rs] - registers.reg[rt];
}

void CPU::op_sw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
uint16_t imm_s = (uint)(int16_t)imm;
    
    uint r = rs;
        uint i = imm_s;
        uint addr = registers.reg[r] + i;

        if ((addr & 0x3) == 0) {
            memory.writeWord(addr,registers.reg[rt])
        }
        else {
          
        }
}

void CPU::op_lh(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value
uint16_t imm_s = (uint)(int16_t)imm;
    uint32_t address = registers.reg[rs] + imm; // Calculate the memory address
    uint32_t value = memory.readWord(address); // Read the word from memory
    uint16_t halfword = static_cast<uint16_t>((value >> ((address & 2) << 3)) & 0xFFFF); // Extract the halfword from the word
    registers.reg[rt] = static_cast<uint32_t>(halfword); // Store the value in the specified register

    uint addr = registers.reg[rs] + imm_s;

        if ((addr & 0x1) == 0) {
            uint value = (uint)(short)memory.readWord(addr);
            registers.reg[rt] = value;
        }
        else {
            
        }
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
    registers.pc = 0xbfc00000; // Start from the beginning of memory
    registers.next_pc = pc + 4;

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

        case 0b100001:
            // lh
            op_lh(instruction);
            console.log("CPU INSTRUCTION :: LH");
            break;

        default:
            Logging console;
            console.warn("Invalid Opcode: " + std::bitset<6>(opcode).to_string());
            break;
        }

        // Program Counter
        registers.pc = registers.next_pc; // 4 Bytes = 32 Bits
        registers.next_pc += 4;

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
