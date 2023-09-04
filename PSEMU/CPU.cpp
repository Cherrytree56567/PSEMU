
/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include "CPU.h"
#pragma warning(disable : 4996)

// First Add rs and rt then store in rd (register)

void CPU::op_add(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    uint8_t rd = (instruction >> 11) & 0x1F; // Extract bits 15 to 11

    std::cout << "ADDING: RESULT = " << result << ", RS = " << std::to_string(rs) << ", RT = " << std::to_string(rt) << ", RD = " << std::to_string(rd) << std::endl;

    registers.reg[rd] = registers.reg[rs] + registers.reg[rt];
}

void CPU::op_storebyte(uint32_t instruction) {
    uint8_t rs = (instruction >> 21) & 0x1F; // Extract bits 25 to 21
    uint8_t rt = (instruction >> 16) & 0x1F; // Extract bits 20 to 16
    int16_t imm = instruction & 0xFFFF;       // Extract the immediate value

    uint32_t rsValue = registers.reg[rs];
    uint8_t valueToStore = rsValue & 0xFF;    // Extract the least significant byte

    // Calculate the effective memory address
    uint32_t effectiveAddress = rsValue + imm;

    // Store the value in memory
    memory[effectiveAddress] = valueToStore;

    std::cout << "STOREBYTE: Value = " << std::to_string(valueToStore) << ", RS = " << std::to_string(rs) << ", Immediate = " << std::to_string(imm) << std::endl;
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
                console.log("CPU INSTRUCTION :: ADD");
                op_add(instruction);
                break;

            case 0b100001:
                // Addu
                console.log("CPU INSTRUCTION :: ADDU");
                break;

            case 0b100100:
                // and
                console.log("CPU INSTRUCTION :: AND");
                break;

            case 0b011010:
                // div
                console.log("CPU INSTRUCTION :: DIV");
                break;

            case 0b011011:
                // divu
                console.log("CPU INSTRUCTION :: DIVU");
                break;

            case 0b001001:
                // jalr
                console.log("CPU INSTRUCTION :: JALR");
                break;

            case 0b001000:
                // jr
                console.log("CPU INSTRUCTION :: JR");
                break;

            case 0b010000:
                // mfhi
                console.log("CPU INSTRUCTION :: MFHI");
                break;

            case 0b010001:
                // mthi
                console.log("CPU INSTRUCTION :: MTHI");
                break;

            case 0b010010:
                // mflo
                console.log("CPU INSTRUCTION :: MFLO");
                break;

            case 0b010011:
                // mtlo
                console.log("CPU INSTRUCTION :: MTLO");
                break;

            case 0b011000:
                // mult
                console.log("CPU INSTRUCTION :: MULT");
                break;

            case 0b011001:
                // multu
                console.log("CPU INSTRUCTION :: MULTU");
                break;

            case 0b100111:
                // nor
                console.log("CPU INSTRUCTION :: NOR");
                break;

            case 0b100110:
                // xor
                console.log("CPU INSTRUCTION :: XOR");
                break;

            case 0b100101:
                // or
                console.log("CPU INSTRUCTION :: OR");
                break;

            case 0b101010:
                // slt
                console.log("CPU INSTRUCTION :: SLT");
                break;

            case 0b101011:
                // sltu
                console.log("CPU INSTRUCTION :: SLTU");
                break;

            case 0b000000:
                // sll
                console.log("CPU INSTRUCTION :: SLL");
                break;

            case 0b000010:
                // srl
                console.log("CPU INSTRUCTION :: SRL");
                break;

            case 0b000011:
                // sra
                console.log("CPU INSTRUCTION :: SRA");
                break;

            case 0b100010:
                // sub
                console.log("CPU INSTRUCTION :: SUB");
                break;

            case 0b100011:
                // subu
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
            console.log("CPU INSTRUCTION :: J");
            break;

        case 0b000011:
            // jal
            console.log("CPU INSTRUCTION :: JAL");
            break;

        // I Type Instructions
        case 0b001000:
            // addi
            console.log("CPU INSTRUCTION :: ADDI");
            break;

        case 0b001001:
            // addiu
            console.log("CPU INSTRUCTION :: ADDIU");
            break;

        case 0b001100:
            // andi
            console.log("CPU INSTRUCTION :: ANDI");
            break;

        case 0b000100:
            // beq
            console.log("CPU INSTRUCTION :: BEQ");
            break;

        case 0b000110:
            // blez
            console.log("CPU INSTRUCTION :: BLEZ");
            break;

        case 0b000101:
            // bne
            console.log("CPU INSTRUCTION :: BNE");
            break;

        case 0b000111:
            // bgtz
            console.log("CPU INSTRUCTION :: BGTZ");
            break;

        case 0b100000:
            // lb
            console.log("CPU INSTRUCTION :: LB");
            break;

        case 0b100100:
            // lbu
            console.log("CPU INSTRUCTION :: LBU");
            break;

        case 0b100101:
            // lhu
            console.log("CPU INSTRUCTION :: LHU");
            break;

        case 0b001111:
            // lui
            console.log("CPU INSTRUCTION :: LUI");
            break;

        case 0b100011:
            // lw
            console.log("CPU INSTRUCTION :: LW");
            break;

        case 0b001101:
            // ori
            console.log("CPU INSTRUCTION :: ORI");
            break;

        case 0b101000:
            // sb
            console.log("CPU INSTRUCTION :: SB");
            break;

        case 0b101001:
            // sh
            console.log("CPU INSTRUCTION :: SH");
            break;

        case 0b001010:
            // slti
            console.log("CPU INSTRUCTION :: SLTI");
            break;

        case 0b001011:
            // sltiu
            console.log("CPU INSTRUCTION :: SLTIU");
            break;

        case 0b101011:
            // sw
            console.log("CPU INSTRUCTION :: SW");
            break;

        default:
            Logging console;
            console.warn("Invalid Opcode: " + std::to_string(opcode));
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
        console.log("MEMORY " + std::to_string(i) + ": " + std::to_string(memory.memory[i]));
    }
}
