#include "CPU.h"

void CPU::tick() {
  fetch();
}

void CPU::fetch() {

    Instruction instr;
    instr.instruction = Next_Instr.instruction;

    Next_Instr.instruction = bus->load32(pc);

    pc += 4;

    set_reg(std::get<0>(load), std::get<1>(load));

    load = std::make_tuple(0, 0);

    decode_execute(instr);
    just_started = false;
    for (int i = 0; i < 32; i++) {
        regs[i] = out_regs[i];
    }
}

void CPU::decode_execute(Instruction instruction) {
    if (just_started == true){
        return;
    }
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
                    
                default:
                    std::cout << "[CPU] ERROR: Unhandled Function Instruction \n";
                    exit(0);
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

        case (0b000111):
            op_bgtz(instruction);
            std::cout << "[CPU] INFO: BGTZ (I-Type)\n";
            break;
            
        case (0b000110):
            op_blez(instruction);
            std::cout << "[CPU] INFO: BLEZ (I-Type)\n";
            break;
            
        default:
            std::cout << "[CPU] ERROR: Unhandled Instruction \n";
            exit(0);
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

    if ((sr & 0x10000) != 0) {
        // Cache is isolated, ignore write
        std::cout << "[CPU] INFO: Ignore store while cache is isolated\n";
        return;
    }

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];
    bus->store32(addr, v);
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

    pc = (pc & 0xf0000000) | (i << 2);
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
    // Offset immediates are always shifted two places to the
    // right since `PC` addresses have to be aligned on 32bits at
    // all times.
    uint32_t offseta = offset << 2;

    pc = pc + offseta;

    // We need to compensate for the hardcoded
    // `pc.wrapping_add(4)` in `run_next_instruction`
    pc = pc - 4;
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

    int32_t s_value = static_cast<int32_t>(regs[s]);

    int32_t v;
    if (i > 0 && s_value > INT32_MAX - i) {
        throw std::overflow_error("ADDI overflow");
    }
    else if (i < 0 && s_value < INT32_MIN - i) {
        throw std::overflow_error("ADDI underflow");
    }
    else {
        v = s_value + i;
    }

    if (v < 0) {
        v = static_cast<uint32_t>(v & 0xFFFFFFFF);  // Ensure v is a positive 32-bit value
    }

    set_reg(t, static_cast<uint32_t>(v));
}

void CPU::op_lw(Instruction instruction) {

    if ((sr & 0x10000) != 0) {
        // Cache is isolated, ignore write
        std::cout << "[CPU] INFO: Ignore load while cache is isolated\n";
        return;
    }

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    uint32_t v = bus->load32(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, v);
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

        if ((sr & 0x10000) != 0) {
            // Cache is isolated, ignore write
            std::cout << "[CPU] INFO: Ignore store while cache is isolated\n";
            return;
        }

        uint32_t i = instruction.imm_s();
        uint32_t t = instruction.rt();
        uint32_t s = instruction.rs();

        uint32_t addr = regs[s] + i;
        uint32_t v    = regs[t];

        bus->store16(addr, (uint16_t)v);
}

void CPU::op_jal(Instruction instruction) {
    uint32_t ra = pc;

    // Store return address in $31 ($ra)
    set_reg(31, ra);

    op_j(instruction);
}

void CPU::op_andi(Instruction instruction) {
    uint32_t i = instruction.imm();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s] & i;

    set_reg(t, v);
}

void CPU::op_sb(Instruction instruction) {

    if ((sr & 0x10000) != 0) {
        // Cache is isolated, ignore write
        std::cout << "[CPU] INFO: Ignore store while cache is isolated\n";
        return;
    }

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;
    uint32_t v = regs[t];

    bus->store8(addr, (uint8_t)v);
}

void CPU::op_jr(Instruction instruction) {
    uint32_t s = instruction.rs();

    pc = regs[s];
}

void CPU::op_lb(Instruction instruction) {

    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    // Cast as i8 to force sign extension
    uint8_t v = bus->load8(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, v);
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
        throw std::runtime_error("[CPU] ERROR: Unhandled read from CAUSE register");
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

    int32_t s_value = static_cast<int32_t>(regs[s]);
    int32_t t_value = static_cast<int32_t>(regs[t]);

    int32_t v;
    if (s_value > 0 && t_value > INT32_MAX - s_value) {
        throw std::overflow_error("ADD overflow");
    }
    else if (s_value < 0 && t_value < INT32_MIN - s_value) {
        throw std::underflow_error("ADD underflow");
    }
    else {
        v = s_value + t_value;
    }

    if (v < 0) {
        v = static_cast<uint32_t>(v & 0xFFFFFFFF);  // Ensure v is a positive 32-bit value
    }

    set_reg(d, static_cast<uint32_t>(v));
}

void CPU::op_bgtz(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s];

    if (v > 0) {
        branch(i);
    }
}

void CPU::op_blez(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t s = instruction.rs();

    uint32_t v = regs[s];

    if (v <= 0) {
        self.branch(i);
    }
}

void CPU::op_lbu(Instruction instruction) {
    uint32_t i = instruction.imm_s();
    uint32_t t = instruction.rt();
    uint32_t s = instruction.rs();

    uint32_t addr = regs[s] + i;

    uint8_t v = self.load8(addr);

    // Put the load in the delay slot
    load = std::make_tuple(t, (uint32_t)v);
}