
/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include "CPU.h"
#pragma warning(disable : 4996)

/* 
* Notes: 
*        Warning: none of the op_* functions have been tested except the op_lui function.
*        TODO: Fix the warning about opcode 51. it gives opcode 51 not found error even though there is no other instruction. I think it is something about the program not exiting properly.
*        TODO: Test the op_add, op_addi, op_and function
*/

// First Add rs and rt then store in rd (register)

void CPU::op_add(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    if (rt > 32) {
        Logging console;
        console.err("Overflow Exception: RT is greater than 32. RT = " + std::to_string(rt));
    } else {
        std::cout << "ADDING: RESULT = " << registers.reg[rs] + registers.reg[rt] << ", RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;

        registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
    }
}

// First take the data from RT (Register) and stores it in IMM + RS (memory)

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
        console.err("Overflow Exception: RT is greater than 32. RT = " + std::to_string(rt));
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

void CPU::op_andi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_beq(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_blez(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_bne(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_bgtz(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_div(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_divu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_j(uint32_t instruction) {
    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_jal(uint32_t instruction) {
    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_jalr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_jr(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_lbu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_lhu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_lw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mfhi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mthi(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mflo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mtlo(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mfc0(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_mult(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_multu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_nor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_xor(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_or(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_ori(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_lb(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sh(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_slt(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_slti(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sltiu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sltu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sll(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_srl(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sra(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sub(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_subu(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
}

void CPU::op_sw(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint16_t imm = instruction & 0xFFFF;      // Extract the immediate value

    std::cout << "Warning: The function for this instruction is not implemented yet. " << std::endl;
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
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
    }

    // Read and separate the content into 32-bit chunks
    size_t bytesRead = fread(aBiosCode, sizeof(uint32_t), numChunks, file);
    if (bytesRead < numChunks) {
        fprintf(stderr, "Error reading file\n");
        free(aBiosCode);
        fclose(file);
    }

    fclose(file);

    BiosCode = aBiosCode;
    this->numInstructions = sizeof(aBiosCode);
}

void CPU::loadBiosCode(uint32_t* binaryCode) {
    this->numInstructions = sizeof(binaryCode) / sizeof(uint32_t);
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
    for (int i = 0; i < memory.memory.size(); ++i) {
        console.log("MEMORY " + std::to_string(i) + ": " + std::bitset<32>(memory.memory[i]).to_string());
    }
}
