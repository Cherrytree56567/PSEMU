#include "CPU.h"

void CPU::tick() {
  fetch();
}

void CPU::reset() {
    pc = 0xbfc00000;
    next_pc = pc + 4;
    current_pc = 0;
    hi = 0xdeadbeef; lo = 0xdeadbeef;
    for (int i = 0; i < 32; i++) {
        regs[i] = 0xdeadbeef;
        out_regs[i] = 0xdeadbeef;
    }
    out_regs[0] = 0;
    regs[0] = 0;
    load = std::make_tuple(0, 0);
    sr = 0;
    cause = 0;
    epc = 0;
    delay_slot = false;
    brancha = false;
}

void CPU::fetch() {
    current_pc = pc;

    if (current_pc % 4 != 0) {
        // PC is not correctly aligned!
        exception(Exception::LoadAddressError);
        return;
    }

    Instruction instr;
    instr.instruction = bus->load32(pc);

    pc = next_pc;
    next_pc = pc + 4;

    set_reg(std::get<0>(load), std::get<1>(load));

    load = std::make_tuple(0, 0);

    delay_slot = brancha;
    brancha = false;

    decode_execute(instr);
    for (int i = 0; i < 32; i++) {
        regs[i] = out_regs[i];
    }
}

void CPU::decode_execute(Instruction instruction) {
    std::cout << "PC: " << pc << "\n";
    switch (instruction.opcode()) {
        case (0b000000):
            switch (instruction.function()) {
                case (0b100101):
                    op_or(instruction);
                    std::cout << "[CPU] INFO: OR (R-Type)\n";
                    break;

                case (0b000000):
                    op_sll(instruction);
                    std::cout << "[CPU] INFO: SLL (R-Type)\n";
                    break;
                    
                case (0b101011):
                    op_sltu(instruction);
                    std::cout << "[CPU] INFO: SLTU (R-Type)\n";
                    break;
                
                case (0b100001):
                    op_addu(instruction);
                    std::cout << "[CPU] INFO: ADDU (R-Type)\n";
                    break;

                case (0b001000):
                    op_jr(instruction);
                    std::cout << "[CPU] INFO: JR (R-Type)\n";
                    break;

                case (0b100100):
                    op_and(instruction);
                    std::cout << "[CPU] INFO: AND (R-Type)\n";
                    break;

                case (0b100000):
                    op_add(instruction);
                    std::cout << "[CPU] INFO: ADD (R-Type)\n";
                    break;
                    
                case (0b001001):
                    op_jalr(instruction);
                    std::cout << "[CPU] INFO: JALR (R-Type)\n";
                    break;
                    
                case (0b100011):
                    op_subu(instruction);
                    std::cout << "[CPU] INFO: SUBU (R-Type)\n";
                    break;
                    
                case (0b000011):
                    op_sra(instruction);
                    std::cout << "[CPU] INFO: SRA (R-Type)\n";
                    break;
                    
                case (0b011010):
                    op_div(instruction);
                    std::cout << "[CPU] INFO: DIV (R-Type)\n";
                    break;
                    
                case (0b010010):
                    op_mflo(instruction);
                    std::cout << "[CPU] INFO: MFLO (R-Type)\n";
                    break;
                    
                case (0b000010):
                    op_srl(instruction);
                    std::cout << "[CPU] INFO: SRL (R-Type)\n";
                    break;
                    
                case (0b011011):
                    op_divu(instruction);
                    std::cout << "[CPU] INFO: DIVU (R-Type)\n";
                    break;
                    
                case (0b010000):
                    op_mfhi(instruction);
                    std::cout << "[CPU] INFO: MFHI (R-Type)\n";
                    break;
                    
                case (0b101010):
                    op_slt(instruction);
                    std::cout << "[CPU] INFO: SLT (R-Type)\n";
                    break;

                case (0b001100):
                    op_syscall(instruction);
                    std::cout << "[CPU] INFO: SYSCALL (R-Type)\n";
                    break;

                case (0b010011):
                    op_mtlo(instruction);
                    std::cout << "[CPU] INFO: MTLO (R-Type)\n";
                    break;

                case (0b010001):
                    op_mthi(instruction);
                    std::cout << "[CPU] INFO: MTHI (R-Type)\n";
                    break;
                    
                case (0b000100):
                    op_sllv(instruction);
                    std::cout << "[CPU] INFO: SLLV (R-Type)\n";
                    break;
                    
                case (0b100111):
                    op_nor(instruction);
                    std::cout << "[CPU] INFO: NOR (R-Type)\n";
                    break;
                    
                case (0b000111):
                    op_srav(instruction);
                    std::cout << "[CPU] INFO: SRAV (R-Type)\n";
                    break;

                case (0b000110):
                    op_srlv(instruction);
                    std::cout << "[CPU] INFO: SRLV (R-Type)\n";
                    break;

                case (0b100110):
                    op_xor(instruction);
                    std::cout << "[CPU] INFO: XOR (R-Type)\n";
                    break;

                case (0b011001):
                    op_multu(instruction);
                    std::cout << "[CPU] INFO: MULTU (R-Type)\n";
                    break;

                case (0b001101):
                    op_break(instruction);
                    std::cout << "[CPU] INFO: BREAK (R-Type)\n";
                    break;

                case (0b011000):
                    op_mult(instruction);
                    std::cout << "[CPU] INFO: MULT (R-Type)\n";
                    break;

                case (0b100010):
                    op_sub(instruction);
                    std::cout << "[CPU] INFO: SUB (R-Type)\n";
                    break;
                    
                default:
                    std::cout << "[CPU] ERROR: Unhandled Function Instruction \n" << instruction.function();
                    exception(Exception::IllegalInstruction);
                    break;
            }
            break;

        case (0b001001):
            op_addiu(instruction);
            std::cout << "[CPU] INFO: ADDIU (I-Type)\n";
            break;

        case (0b000010):
            op_j(instruction);
            std::cout << "[CPU] INFO: J (J-Type)\n";
            break;

        case (0b001111):
            op_lui(instruction);
            std::cout << "[CPU] INFO: LUI (I-Type)\n";
            break;

        case (0b001101):
            op_ori(instruction);
            std::cout << "[CPU] INFO: ORI (I-Type)\n";
            break;

        case (0b101011):
            op_sw(instruction);
            std::cout << "[CPU] INFO: SW (I-Type)\n";
            break;

        case (0b010000):
            op_cop0(instruction);
            std::cout << "[CPU] INFO: COP0 (I-Type)\n";
            break;

        case (0b000101):
            op_bne(instruction);
            std::cout << "[CPU] INFO: BNE (I-Type)\n";
            break;

        case (0b001000):
            op_addi(instruction);
            std::cout << "[CPU] INFO: ADDI (I-Type)\n";
            break;

        case (0b100011):
            op_lw(instruction);
            std::cout << "[CPU] INFO: LW (I-Type)\n";
            break;
        
        case (0b101001):
            op_sh(instruction);
            std::cout << "[CPU] INFO: SH (I-Type)\n";
            break;

        case (0b000011):
            op_jal(instruction);
            std::cout << "[CPU] INFO: JAL (I-Type)\n";
            break;

        case (0b001100):
            op_andi(instruction);
            std::cout << "[CPU] INFO: ANDI (I-Type)\n";
            break;

        case (0b101000):
            op_sb(instruction);
            std::cout << "[CPU] INFO: SB (I-Type)\n";
            break;

        case (0b100000):
            op_lb(instruction);
            std::cout << "[CPU] INFO: LB (I-Type)\n";
            break;

        case (0b000100):
            op_beq(instruction);
            std::cout << "[CPU] INFO: BEQ (I-Type)\n";
            break;
            
        case (0b000001):
            op_bxx(instruction);
            std::cout << "[CPU] INFO: BXX (I-Type)\n";
            break;

        case (0b000111):
            op_bgtz(instruction);
            std::cout << "[CPU] INFO: BGTZ (I-Type)\n";
            break;
            
        case (0b000110):
            op_blez(instruction);
            std::cout << "[CPU] INFO: BLEZ (I-Type)\n";
            break;
            
        case (0b100100):
            op_lbu(instruction);
            std::cout << "[CPU] INFO: LBU (I-Type)\n";
            break;
            
        case (0b001010):
            op_slti(instruction);
            std::cout << "[CPU] INFO: SLTI (I-Type)\n";
            break;
            
        case (0b001011):
            op_sltiu(instruction);
            std::cout << "[CPU] INFO: SLTIU (I-Type)\n";
            break;
            
        case (0b100101):
            op_lhu(instruction);
            std::cout << "[CPU] INFO: LHU (I-Type)\n";
            break;
            
        case (0b100001):
            op_lh(instruction);
            std::cout << "[CPU] INFO: LH (I-Type)\n";
            break;

        case (0b001110):
            op_xori(instruction);
            std::cout << "[CPU] INFO: XORI (I-Type)\n";
            break;

        case (0b010001):
            op_cop1(instruction);
            std::cout << "[CPU] INFO: COP1 (I-Type)\n";
            break;

        case (0b010011):
            op_cop3(instruction);
            std::cout << "[CPU] INFO: COP3 (I-Type)\n";
            break;

        case (0b010010):
            op_cop2(instruction);
            std::cout << "[CPU] INFO: COP2 (I-Type)\n";
            break;

        case (0b100010):
            op_lwl(instruction);
            std::cout << "[CPU] INFO: LWL (I-Type)\n";
            break;

        case (0b100110):
            op_lwr(instruction);
            std::cout << "[CPU] INFO: LWR (I-Type)\n";
            break;

        case (0b101010):
            op_swl(instruction);
            std::cout << "[CPU] INFO: SWL (I-Type)\n";
            break;

        case (0b101110):
            op_swr(instruction);
            std::cout << "[CPU] INFO: SWR (I-Type)\n";
            break;

        case (0b110000):
            op_lwc0(instruction);
            std::cout << "[CPU] INFO: LWC0 (I-Type)\n";
            break;

        case (0b110001):
            op_lwc1(instruction);
            std::cout << "[CPU] INFO: LWC1 (I-Type)\n";
            break;

        case (0b110010):
            op_lwc2(instruction);
            std::cout << "[CPU] INFO: LWC2 (I-Type)\n";
            break;

        case (0b110011):
            op_lwc3(instruction);
            std::cout << "[CPU] INFO: LWC3 (I-Type)\n";
            break;

        case (0b111000):
            op_swc0(instruction);
            std::cout << "[CPU] INFO: SWC0 (I-Type)\n";
            break;

        case (0b111001):
            op_swc1(instruction);
            std::cout << "[CPU] INFO: SWC1 (I-Type)\n";
            break;

        case (0b111010):
            op_swc2(instruction);
            std::cout << "[CPU] INFO: SWC2 (I-Type)\n";
            break;

        case (0b111011):
            op_swc3(instruction);
            std::cout << "[CPU] INFO: SWC3 (I-Type)\n";
            break;

        default:
            std::cout << "[CPU] ERROR: Unhandled Instruction \n";
            exception(Exception::IllegalInstruction);
            break;
    }
}

void CPU::op_lui(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();

    uint32_t v = i << 16;

    set_reg(t, v);
}

void CPU::op_ori(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] | i;

    set_reg(t, v);
}

void CPU::op_sw(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    // Assuming reg is a function that returns the value in a register
    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    if (addr % 4 == 0) {
        store32(addr, v);
    }
    else {
        exception(Exception::StoreAddressError);
    }
}

void CPU::op_sll(Instruction instruction) {
    uint32_t i = instruction.sa();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    uint32_t v = regs[t] << i;

    set_reg(d, v);
}

void CPU::op_addiu(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] + i;

    set_reg(t, v);
}

void CPU::op_j(Instruction instruction) {
    uint32_t i = instruction.addr();

    next_pc = (pc & 0xf0000000) | (i << 2);

    brancha = true;
}

void CPU::op_or(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = regs[s] | regs[t];

    set_reg(d, v);
}

void CPU::op_cop0(Instruction instruction) {
    switch (instruction.rs()) {
        case (0b00100):
            op_mtc0(instruction);
            std::cout << "[CPU] INFO: MTC0 (COP0-Type)\n";
            break;

        case (0b00000):
            op_mfc0(instruction);
            std::cout << "[CPU] INFO: MFC0 (COP0-Type)\n";
            break;

        case (0b10000):
            op_rfe(instruction);
            std::cout << "[CPU] INFO: RFE (COP0-Type)\n";
            break;

        default:
            throw std::runtime_error("[CPU] ERROR: Unhandled COP0 Instruction");
            break;
    }
}

void CPU::op_mtc0(Instruction instruction) {
    uint32_t cpu_r = instruction.rt();
    uint32_t cop_r = instruction.rd();

    uint32_t v = regs[cpu_r];

    switch (cop_r) {
        case 3:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 5:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 6:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 7:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 9:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 11:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to COP0 Register" + std::to_string(cop_r));
            }
            break;

        case 12:
            sr = v;
            break;

        case 13:
            if (v != 0) {
                throw std::runtime_error("[CPU] ERROR: Unhandled write to CAUSE register.");
            }
            break;

        default:
            throw std::runtime_error("Unhandled cop0 register " + std::to_string(cop_r));
            break;
    }
}

void CPU::branch(uint32_t offset) {
    uint32_t offseta = offset << 2;

    next_pc = pc + offseta;

    brancha = true;
}

void CPU::op_bne(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    if (regs[s] != regs[t]) {
        branch(i);
    }
}

void CPU::op_addi(Instruction instruction) {
    int32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    int32_t sValue = regs[s];

    int32_t v;

    // Manual overflow check
    if ((i > 0 && sValue > INT_MAX - i) || (i < 0 && sValue < INT_MIN - i)) {
        // Overflow occurred
        exception(Exception::Overflow);
    }
    else {
        v = sValue + i;
        set_reg(t, static_cast<uint32_t>(v));
    }
}

void CPU::op_lw(Instruction instruction) {

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    if (addr % 4 == 0) {
        uint32_t v = bus->load32(addr);

        // Put the load in the delay slot
        load = std::make_tuple(t, v);
    }
    else {
        exception(Exception::LoadAddressError);
    }
}

void CPU::op_sltu(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = regs[s] < regs[t];

    set_reg(d, v);
}

void CPU::op_addu(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    uint32_t v = regs[s] + regs[t];

    set_reg(d, v);
}

void CPU::op_sh(Instruction instruction) {
        uint32_t i = instruction.imm_s();
        uint32_t t = instruction.rt();
        uint32_t s = instruction.rs();

        uint32_t addr = regs[s] + i;
        uint32_t v    = regs[t];

        if (addr % 2 == 0) {
            store16(addr, (uint16_t)v);
        }
        else {
            exception(Exception::StoreAddressError);
        }
}

void CPU::op_jal(Instruction instruction) {
    uint32_t ra = next_pc;

    // Store return address in $31 ($ra)
    set_reg(31, ra);

    op_j(instruction);
    brancha = true;
}

void CPU::op_andi(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] & i;

    set_reg(t, v);
}

void CPU::op_sb(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    store8(addr, (uint8_t)v);
}

void CPU::op_jr(Instruction instruction) {
    uint32_t s = instruction.rs();

    next_pc = regs[s];
    
    brancha = true;
}

void CPU::op_lb(Instruction instruction) {

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // Cast as i8 to force sign extension
    uint8_t v = bus->load8(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, (uint8_t)v);
}

void CPU::op_beq(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    if (regs[s] == regs[t]) {
        branch(i);
    }
}

void CPU::op_mfc0(Instruction instruction) {
    uint32_t cpu_r = instruction.rt();
    uint32_t cop_r = instruction.rd();

    uint32_t v;

    switch (cop_r) {
    case 12:
        v = sr;
        break;
    case 13:
        v = cause;
        break;
    case 14:
        v = epc;
        break;
    default:
        throw std::runtime_error("[CPU] ERROR: Unhandled read from COP0R" + std::to_string(cop_r));
    }

    load = std::make_tuple(cpu_r, v);
}

void CPU::op_and(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = regs[s] & regs[t];

    set_reg(d, v);
}

void CPU::op_add(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    int32_t sValue = regs[s];
    int32_t tValue = regs[t];

    // Check for integer overflow
    if ((tValue > 0 && sValue > INT_MAX - tValue) || (tValue < 0 && sValue < INT_MIN - tValue)) {
        // Overflow occurred
        exception(Exception::Overflow);
    }
    else {
        set_reg(d, static_cast<uint32_t>(sValue + tValue));
    }

}

void CPU::op_bgtz(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();

    int32_t v = regs[s];

    if (v > 0) {
        branch(i);
    }
}

void CPU::op_blez(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();

    int32_t v = regs[s];

    if (v <= 0) {
        branch(i);
    }
}

void CPU::op_lbu(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    uint8_t v = bus->load8(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, (uint32_t)v);
}

void CPU::op_jalr(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();

    uint32_t ra = next_pc;

    // Store return address in `d`
    set_reg(d, ra);

    next_pc = regs[s];
    brancha = true;
}

void CPU::op_bxx(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();

    uint32_t instructiona = instruction.instruction;

    uint32_t is_bgez = (instructiona >> 16) & 1;
    uint32_t is_link = (instructiona >> 20) & 1 != 0;

    int32_t v = regs[s];

    // Test "less than zero"
    uint32_t test = (v < 0);

    // If the test is "greater than or equal to zero" we need
    // to negate the comparison above since
    // ("a >= 0" <=> "!(a < 0)"). The xor takes care of that.
    test = test ^ is_bgez;
    
    if (test != 0) {
        if (is_link) {
            uint32_t ra = next_pc;

            // Store return address in R31
            set_reg(31, ra);
        }
        branch(i);
    }
}

void CPU::op_slti(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = ((int32_t)regs[s]) < i;

    set_reg(t, v);
}

void CPU::op_subu(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    uint32_t v = regs[s] - regs[t];

    set_reg(d, v);
}

void CPU::op_sra(Instruction instruction) {
    uint32_t i = instruction.sa();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    uint32_t v = ((int32_t)regs[t]) >> i;

    set_reg(d, v);
}

void CPU::op_div(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t n = regs[s];
    uint32_t d = regs[t];

    if (d == 0) {
        // Division by zero, results are bogus
        hi = static_cast<uint32_t>(n);

        if (n >= 0) {
            lo = 0xffffffff;
        } else {
            lo = 1;
        }
    } else if (static_cast<uint32_t>(n) == 0x80000000 && d == -1) {
        // Result is not representable in a 32-bit signed integer
        hi = 0;
        lo = 0x80000000;
    } else {
        hi = static_cast<uint32_t>(n % d);
        lo = static_cast<uint32_t>(n / d);
    }
}

void CPU::op_mflo(Instruction instruction) {
    uint32_t d = instruction.rd();

    set_reg(d, lo);
}

void CPU::op_srl(Instruction instruction) {
    uint32_t i = instruction.sa();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    uint32_t v = regs[t] >> i;

    set_reg(d, v);
}

void CPU::op_sltiu(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = regs[s] < i;

    set_reg(t, v);
}

void CPU::op_divu(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t n = regs[s];
    uint32_t d = regs[t];

    if (d == 0) {
        // Division by zero, results are bogus
        hi = n;
        lo = 0xffffffff;
    } else {
        hi = n % d;
        lo = n / d;
    }
}

void CPU::op_mfhi(Instruction instruction) {
    uint32_t d = instruction.rd();

    set_reg(d, hi);
}

void CPU::op_slt(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    int32_t sa = regs[s];
    int32_t ta = regs[t];

    int32_t v = s < t;

    set_reg(d, (uint32_t)v);
}

void CPU::exception(Exception causea) {
    if (regs[1] == 10) exit(0);
    uint32_t handler = (sr & (1 << 22)) ? 0xbfc00180 : 0x80000080;

    uint32_t mode = sr & 0x3F;
    sr &= (uint32_t)0x3F;
    sr |= (mode << 2) & 0x3F;

    // Update `CAUSE` register with the exception code (bits [6:2])
    cause = (static_cast<uint32_t>(causea) << 2);

    epc = current_pc;

    if (delay_slot) {
        epc = epc - 4;
        cause |= 1 << 31;
    }

    // Exceptions don't have a branch delay slot; we jump directly
    // into the handler.
    pc = handler;
    next_pc = pc + 4;
}

void CPU::op_syscall(Instruction instruction) {
    exception(Exception::SysCall);
}

void CPU::op_mtlo(Instruction instruction) {
    uint32_t s = instruction.rs();

    lo = regs[s];
}

void CPU::op_mthi(Instruction instruction) {
    uint32_t s = instruction.rs();

    hi = regs[s];
}

void CPU::op_rfe(Instruction instruction) {
    if ((instruction.instruction & 0x3f) != 0b010000) {
        std::cout << "[CPU] ERROR: Invalid cop0 instruction: " << std::to_string(instruction.instruction) << "\n";
    }

    // Restore the pre-exception mode by shifting the Interrupt
    // Enable/User Mode stack back to its original position.
    uint32_t mode = sr & 0x3f;
    sr &= !0x3f;
    sr |= mode >> 2;
}

void CPU::op_lhu(Instruction instruction) {

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // Address must be 16bit aligned
    if (addr % 2 == 0) {
        uint16_t v = bus->load16(addr);

        // Put the load in the delay slot
        load = std::make_tuple(t, (uint32_t)v);
    } else {
        exception(Exception::LoadAddressError);
    }
}

void CPU::op_sllv(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    // Shift amount is truncated to 5 bits
    uint32_t v = regs[t] << (regs[s] & 0x1f);

    set_reg(d, v);
}

void CPU::op_lh(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // Cast as i16 to force sign extension
    uint16_t v = bus->load16(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, (uint32_t)v);
}

void CPU::op_nor(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = ~(regs[s] | regs[t]);

    set_reg(d, v);
}

void CPU::op_srav(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    // Shift amount is truncated to 5 bits
    uint32_t v = ((int32_t)regs[t]) >> (regs[s] & 0x1f);

    set_reg(d, v);
}

void CPU::op_srlv(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    // Shift amount is truncated to 5 bits
    uint32_t v = regs[t] >> (regs[s] & 0x1f);

    set_reg(d, v);
}

void CPU::op_multu(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint64_t a = regs[s];
    uint64_t b = regs[t];

    uint32_t v = a * b;

    hi = (uint32_t)(v >> 32);
    lo = (uint32_t)v;
}

void CPU::op_xor(Instruction instruction) {
    uint32_t d = instruction.rd();
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint32_t v = regs[s] ^ regs[t];

    set_reg(d, v);
}

void CPU::op_break(Instruction instruction) {
    exception(Exception::Break);
}

void CPU::op_mult(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();

    uint64_t a = ((uint32_t)regs[s]);
    uint64_t b = ((uint32_t)regs[t]);

    uint64_t v = (a * b);

    hi = (uint32_t)(v >> 32);
    lo = (uint32_t)v;
}

void CPU::op_sub(Instruction instruction) {
    uint32_t s = instruction.rs();
    uint32_t t = instruction.rt();
    uint32_t d = instruction.rd();

    int32_t sValue = regs[s];
    int32_t tValue = regs[t];

    // Check for integer underflow
    if ((tValue > 0 && sValue < INT_MIN + tValue) || (tValue < 0 && sValue > INT_MAX + tValue)) {
        // Underflow occurred
        exception(Exception::Overflow);
    }
    else {
        set_reg(d, static_cast<uint32_t>(sValue - tValue));
    }
}

void CPU::op_xori(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] ^ i;

    set_reg(t, v);
}

void CPU::op_cop1(Instruction) {
    exception(Exception::CoprocessorError);
}

void CPU::op_cop3(Instruction) {
    exception(Exception::CoprocessorError);
}

void CPU::op_cop2(Instruction instruction) {
    std::cout << "Unhandled GTE instruction: {" << std::to_string(instruction.instruction);
}

void CPU::op_lwl(Instruction instruction) {
    int32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // This instruction bypasses the load delay restriction: this
    // instruction will merge the new contents with the value
    // currently being loaded if need be.
    uint32_t cur_v = out_regs[t];

    // Next we load the *aligned* word containing the first
    // addressed byte
    uint32_t aligned_addr = addr & ~3U;
    uint32_t aligned_word = bus->load32(aligned_addr);

    // Depending on the address alignment, we fetch the 1, 2, 3, or
    // 4 *most* significant bytes and put them in the target register.
    uint32_t v;
    switch (addr & 3U) {
    case 0:
        v = (cur_v & 0x00FFFFFF) | (aligned_word << 24);
        break;
    case 1:
        v = (cur_v & 0x0000FFFF) | (aligned_word << 16);
        break;
    case 2:
        v = (cur_v & 0x000000FF) | (aligned_word << 8);
        break;
    case 3:
        v = (cur_v & 0x00000000) | (aligned_word);
        break;
    default:
        throw std::runtime_error("Unreachable code");
    }

    // Put the load in the delay slot
    load = std::make_tuple(t, v);
}


void CPU::op_lwr(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // This instruction bypasses the load delay restriction: this
    // instruction will merge the new contents with the value
    // currently being loaded if need be.
    uint32_t cur_v = out_regs[t];

    // Next, we load the *aligned* word containing the first
    // addressed byte
    uint32_t aligned_addr = addr & ~3;
    uint32_t aligned_word = bus->load32(aligned_addr);

    // Depending on the address alignment, we fetch the 1, 2, 3, or
    // 4 *least* significant bytes and put them in the target
    // register.
    uint32_t v = 0;
    switch (addr & 3) {
        case 0:
            v = (cur_v & 0x00000000) | (aligned_word >> 0);
            break;
        case 1:
            v = (cur_v & 0xff000000) | (aligned_word >> 8);
            break;
        case 2:
            v = (cur_v & 0xffff0000) | (aligned_word >> 16);
            break;
        case 3:
            v = (cur_v & 0xffffff00) | (aligned_word >> 24);
            break;
        default:
            // Handle any unexpected cases here
            break;
    }

    // Put the load in the delay slot
    load = std::make_tuple(t, v);
}

void CPU::op_swl(Instruction instruction) {
    int32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    uint32_t aligned_addr = addr & ~3;

    // Load the current value for the aligned word at the target address
    uint32_t cur_mem = bus->load32(aligned_addr);

    uint32_t mem;
    switch (addr & 3U) {
    case 0:
        mem = (cur_mem & 0xFFFFFF00) | (v >> 24);
        break;
    case 1:
        mem = (cur_mem & 0xFFFF0000) | (v >> 16);
        break;
    case 2:
        mem = (cur_mem & 0xFF000000) | (v >> 8);
        break;
    case 3:
        mem = (cur_mem & 0x00000000) | (v >> 0);
        break;
    default:
        throw std::runtime_error("Unreachable code");
    }

    store32(aligned_addr, mem);
}

void CPU::op_swr(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    uint32_t aligned_addr = addr & ~3;

    // Load the current value for the aligned word at the target address
    uint32_t cur_mem = bus->load32(aligned_addr);

    uint32_t mem;
    switch (addr & 3U) {
    case 0:
        mem = (cur_mem & 0x00000000) | (v << 0);
        break;
    case 1:
        mem = (cur_mem & 0x000000FF) | (v << 8);
        break;
    case 2:
        mem = (cur_mem & 0x0000FFFF) | (v << 16);
        break;
    case 3:
        mem = (cur_mem & 0x00FFFFFF) | (v << 24);
        break;
    default:
        throw std::runtime_error("Unreachable code");
    }

    store32(aligned_addr, mem);
}

void CPU::op_lwc0(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_lwc1(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_lwc2(Instruction instruction) {
    // Handle the GTE LWC instruction. You can customize this part.
    // If not supported, you can throw an exception like in the Rust code.
    // For example:
    throw std::runtime_error("Unhandled GTE LWC: " + std::to_string(instruction.instruction));
}

void CPU::op_lwc3(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc0(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc1(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc2(Instruction instruction) {
    // Handle the GTE SWC instruction. You can customize this part.
    // If not supported, you can throw an exception like in the Rust code.
    // For example:
    throw std::runtime_error("Unhandled GTE SWC: " + std::to_string(instruction.instruction));
}

void CPU::op_swc3(Instruction instruction) {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}
