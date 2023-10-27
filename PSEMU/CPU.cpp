#include "CPU.h"

bool sub_overflow(uint32_t old_value, uint32_t sub_value, uint32_t new_value) {
    return (((new_value ^ old_value) & (old_value ^ sub_value)) & UINT32_C(0x80000000)) != 0;
}

bool add_overflow(uint32_t old_value, uint32_t add_value, uint32_t new_value) {
    return (((new_value ^ old_value) & (new_value ^ add_value)) & UINT32_C(0x80000000)) != 0;
}

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
                    op_or();
                    std::cout << "[CPU] INFO: OR (R-Type)\n";
                    break;

                case (0b000000):
                    op_sll();
                    std::cout << "[CPU] INFO: SLL (R-Type)\n";
                    break;
                    
                case (0b101011):
                    op_sltu();
                    std::cout << "[CPU] INFO: SLTU (R-Type)\n";
                    break;
                
                case (0b100001):
                    op_addu();
                    std::cout << "[CPU] INFO: ADDU (R-Type)\n";
                    break;

                case (0b001000):
                    op_jr();
                    std::cout << "[CPU] INFO: JR (R-Type)\n";
                    break;

                case (0b100100):
                    op_and();
                    std::cout << "[CPU] INFO: AND (R-Type)\n";
                    break;

                case (0b100000):
                    op_add();
                    std::cout << "[CPU] INFO: ADD (R-Type)\n";
                    break;
                    
                case (0b001001):
                    op_jalr();
                    std::cout << "[CPU] INFO: JALR (R-Type)\n";
                    break;
                    
                case (0b100011):
                    op_subu();
                    std::cout << "[CPU] INFO: SUBU (R-Type)\n";
                    break;
                    
                case (0b000011):
                    op_sra();
                    std::cout << "[CPU] INFO: SRA (R-Type)\n";
                    break;
                    
                case (0b011010):
                    op_div();
                    std::cout << "[CPU] INFO: DIV (R-Type)\n";
                    break;
                    
                case (0b010010):
                    op_mflo();
                    std::cout << "[CPU] INFO: MFLO (R-Type)\n";
                    break;
                    
                case (0b000010):
                    op_srl();
                    std::cout << "[CPU] INFO: SRL (R-Type)\n";
                    break;
                    
                case (0b011011):
                    op_divu();
                    std::cout << "[CPU] INFO: DIVU (R-Type)\n";
                    break;
                    
                case (0b010000):
                    op_mfhi();
                    std::cout << "[CPU] INFO: MFHI (R-Type)\n";
                    break;
                    
                case (0b101010):
                    op_slt();
                    std::cout << "[CPU] INFO: SLT (R-Type)\n";
                    break;

                case (0b001100):
                    op_syscall();
                    std::cout << "[CPU] INFO: SYSCALL (R-Type)\n";
                    break;

                case (0b010011):
                    op_mtlo();
                    std::cout << "[CPU] INFO: MTLO (R-Type)\n";
                    break;

                case (0b010001):
                    op_mthi();
                    std::cout << "[CPU] INFO: MTHI (R-Type)\n";
                    break;
                    
                case (0b000100):
                    op_sllv();
                    std::cout << "[CPU] INFO: SLLV (R-Type)\n";
                    break;
                    
                case (0b100111):
                    op_nor();
                    std::cout << "[CPU] INFO: NOR (R-Type)\n";
                    break;
                    
                case (0b000111):
                    op_srav();
                    std::cout << "[CPU] INFO: SRAV (R-Type)\n";
                    break;

                case (0b000110):
                    op_srlv();
                    std::cout << "[CPU] INFO: SRLV (R-Type)\n";
                    break;

                case (0b100110):
                    op_xor();
                    std::cout << "[CPU] INFO: XOR (R-Type)\n";
                    break;

                case (0b011001):
                    op_multu();
                    std::cout << "[CPU] INFO: MULTU (R-Type)\n";
                    break;

                case (0b001101):
                    op_break();
                    std::cout << "[CPU] INFO: BREAK (R-Type)\n";
                    break;

                case (0b011000):
                    op_mult();
                    std::cout << "[CPU] INFO: MULT (R-Type)\n";
                    break;

                case (0b100010):
                    op_sub();
                    std::cout << "[CPU] INFO: SUB (R-Type)\n";
                    break;
                    
                default:
                    std::cout << "[CPU] ERROR: Unhandled Function Instruction \n" << instruction.function();
                    exception(Exception::IllegalInstruction);
                    break;
            }
            break;

        case (0b001001):
            op_addiu();
            std::cout << "[CPU] INFO: ADDIU (I-Type)\n";
            break;

        case (0b000010):
            op_j();
            std::cout << "[CPU] INFO: J (J-Type)\n";
            break;

        case (0b001111):
            op_lui();
            std::cout << "[CPU] INFO: LUI (I-Type)\n";
            break;

        case (0b001101):
            op_ori();
            std::cout << "[CPU] INFO: ORI (I-Type)\n";
            break;

        case (0b101011):
            op_sw();
            std::cout << "[CPU] INFO: SW (I-Type)\n";
            break;

        case (0b010000):
            op_cop0();
            std::cout << "[CPU] INFO: COP0 (I-Type)\n";
            break;

        case (0b000101):
            op_bne();
            std::cout << "[CPU] INFO: BNE (I-Type)\n";
            break;

        case (0b001000):
            op_addi();
            std::cout << "[CPU] INFO: ADDI (I-Type)\n";
            break;

        case (0b100011):
            op_lw();
            std::cout << "[CPU] INFO: LW (I-Type)\n";
            break;
        
        case (0b101001):
            op_sh();
            std::cout << "[CPU] INFO: SH (I-Type)\n";
            break;

        case (0b000011):
            op_jal();
            std::cout << "[CPU] INFO: JAL (I-Type)\n";
            break;

        case (0b001100):
            op_andi();
            std::cout << "[CPU] INFO: ANDI (I-Type)\n";
            break;

        case (0b101000):
            op_sb();
            std::cout << "[CPU] INFO: SB (I-Type)\n";
            break;

        case (0b100000):
            op_lb();
            std::cout << "[CPU] INFO: LB (I-Type)\n";
            break;

        case (0b000100):
            op_beq();
            std::cout << "[CPU] INFO: BEQ (I-Type)\n";
            break;
            
        case (0b000001):
            op_bxx();
            std::cout << "[CPU] INFO: BXX (I-Type)\n";
            break;

        case (0b000111):
            op_bgtz();
            std::cout << "[CPU] INFO: BGTZ (I-Type)\n";
            break;
            
        case (0b000110):
            op_blez();
            std::cout << "[CPU] INFO: BLEZ (I-Type)\n";
            break;
            
        case (0b100100):
            op_lbu();
            std::cout << "[CPU] INFO: LBU (I-Type)\n";
            break;
            
        case (0b001010):
            op_slti();
            std::cout << "[CPU] INFO: SLTI (I-Type)\n";
            break;
            
        case (0b001011):
            op_sltiu();
            std::cout << "[CPU] INFO: SLTIU (I-Type)\n";
            break;
            
        case (0b100101):
            op_lhu();
            std::cout << "[CPU] INFO: LHU (I-Type)\n";
            break;
            
        case (0b100001):
            op_lh();
            std::cout << "[CPU] INFO: LH (I-Type)\n";
            break;

        case (0b001110):
            op_xori();
            std::cout << "[CPU] INFO: XORI (I-Type)\n";
            break;

        case (0b010001):
            op_cop1();
            std::cout << "[CPU] INFO: COP1 (I-Type)\n";
            break;

        case (0b010011):
            op_cop3();
            std::cout << "[CPU] INFO: COP3 (I-Type)\n";
            break;

        case (0b010010):
            op_cop2();
            std::cout << "[CPU] INFO: COP2 (I-Type)\n";
            break;

        case (0b100010):
            op_lwl();
            std::cout << "[CPU] INFO: LWL (I-Type)\n";
            break;

        case (0b100110):
            op_lwr();
            std::cout << "[CPU] INFO: LWR (I-Type)\n";
            break;

        case (0b101010):
            op_swl();
            std::cout << "[CPU] INFO: SWL (I-Type)\n";
            break;

        case (0b101110):
            op_swr();
            std::cout << "[CPU] INFO: SWR (I-Type)\n";
            break;

        case (0b110000):
            op_lwc0();
            std::cout << "[CPU] INFO: LWC0 (I-Type)\n";
            break;

        case (0b110001):
            op_lwc1();
            std::cout << "[CPU] INFO: LWC1 (I-Type)\n";
            break;

        case (0b110010):
            op_lwc2();
            std::cout << "[CPU] INFO: LWC2 (I-Type)\n";
            break;

        case (0b110011):
            op_lwc3();
            std::cout << "[CPU] INFO: LWC3 (I-Type)\n";
            break;

        case (0b111000):
            op_swc0();
            std::cout << "[CPU] INFO: SWC0 (I-Type)\n";
            break;

        case (0b111001):
            op_swc1();
            std::cout << "[CPU] INFO: SWC1 (I-Type)\n";
            break;

        case (0b111010):
            op_swc2();
            std::cout << "[CPU] INFO: SWC2 (I-Type)\n";
            break;

        case (0b111011):
            op_swc3();
            std::cout << "[CPU] INFO: SWC3 (I-Type)\n";
            break;

        default:
            std::cout << "[CPU] ERROR: Unhandled Instruction \n";
            exception(Exception::IllegalInstruction);
            break;
    }
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

void CPU::op_bxx()
{
    brancha = true;
    uint32_t op = instr.rt();

    bool should_link = (op & 0x1E) == 0x10;
    bool should_branch = (int)(regs[instr.rs()] ^ (op << 31)) < 0;

    if (should_link) regs[31] = next_pc;
    if (should_branch) branch();
}

void CPU::op_swr()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    uint32_t aligned_addr = addr & 0xFFFFFFFC;
    uint32_t aligned_load = bus->load32(aligned_addr);

    uint32_t value = 0;
    switch (addr & 0b11) {
    case 0:
        value = regs[instr.rt()]; break;
    case 1:
        value = (aligned_load & 0x000000FF) | (regs[instr.rt()] << 8);
        break;
    case 2:
        value = (aligned_load & 0x0000FFFF) | (regs[instr.rt()] << 16);
        break;
    case 3:
        value = (aligned_load & 0x00FFFFFF) | (regs[instr.rt()] << 24);
        break;
    }

    bus->store32(aligned_addr, value);
}

void CPU::op_swl()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    uint32_t aligned_addr = addr & 0xFFFFFFFC;
    uint32_t aligned_load = bus->load32(aligned_addr);

    uint32_t value = 0;
    switch (addr & 0b11) {
    case 0:
        value = (aligned_load & 0xFFFFFF00) | (regs[instr.rt()] >> 24);
        break;
    case 1:
        value = (aligned_load & 0xFFFF0000) | (regs[instr.rt()] >> 16);
        break;
    case 2:
        value = (aligned_load & 0xFF000000) | (regs[instr.rt()] >> 8);
        break;
    case 3:
        value = regs[instr.rt()]; break;
    }

    bus->store32(aligned_addr, value);
}

void CPU::op_lwr()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    uint32_t aligned_addr = addr & 0xFFFFFFFC;
    uint32_t aligned_load = bus->load32(aligned_addr);

    uint32_t value = 0;
    uint32_t LRValue = regs[instr.rt()];

    if (instr.rt() == std::get<0>(load)) {
        LRValue = std::get<1>(load);
    }

    switch (addr & 0b11) {
    case 0:
        value = aligned_load;
        break;
    case 1:
        value = (LRValue & 0xFF000000) | (aligned_load >> 8);
        break;
    case 2:
        value = (LRValue & 0xFFFF0000) | (aligned_load >> 16);
        break;
    case 3:
        value = (LRValue & 0xFFFFFF00) | (aligned_load >> 24);
        break;
    }

    load = std::make_tuple(instr.rt(), value);
}

void CPU::op_lwl()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    uint32_t aligned_addr = addr & 0xFFFFFFFC;
    uint32_t aligned_load = bus->load32(aligned_addr);

    uint32_t value = 0;
    uint32_t LRValue = regs[instr.rt()];

    if (instr.rt() == std::get<0>(load)) {
        LRValue = std::get<1>(load);
    }

    switch (addr & 0b11) {
    case 0:
        value = (LRValue & 0x00FFFFFF) | (aligned_load << 24);
        break;
    case 1:
        value = (LRValue & 0x0000FFFF) | (aligned_load << 16);
        break;
    case 2:
        value = (LRValue & 0x000000FF) | (aligned_load << 8);
        break;
    case 3:
        value = aligned_load;
        break;
    }

    load = std::make_tuple(instr.rt(), value);
}

void CPU::op_xori()
{
    set_reg(instr.rt(), regs[instr.rs()] ^ instr.imm());
}

void CPU::op_sub()
{
    uint32_t rs = regs[instr.rs()];
    uint32_t rt = regs[instr.rt()];
    uint32_t sub = rs - rt;

    bool overflow = sub_overflow(rs, rt, sub);
    if (!overflow)
        set_reg(instr.rd(), sub);
    else
        exception(Exception::Overflow);
}

void CPU::op_mult()
{
    int64_t value = (int64_t)(int)regs[instr.rs()] * (int64_t)(int)regs[instr.rt()]; //sign extend to pass amidog cpu test

    hi = (uint32_t)(value >> 32);
    lo = (uint32_t)value;
}

void CPU::op_break()
{
    exception(Exception::Break);
}

void CPU::op_xor()
{
    set_reg(instr.rd(), regs[instr.rs()] ^ regs[instr.rt()]);
}

void CPU::op_multu()
{
    uint64_t value = (uint64_t)regs[instr.rs()] * (uint64_t)regs[instr.rt()]; //sign extend to pass amidog cpu test

    hi = (uint32_t)(value >> 32);
    lo = (uint32_t)value;
}

void CPU::op_srlv()
{
    set_reg(instr.rd(), regs[instr.rt()] >> (int)(regs[instr.rs()] & 0x1F));
}

void CPU::op_srav()
{
    set_reg(instr.rd(), (uint32_t)((int)regs[instr.rt()] >> (int)(regs[instr.rs()] & 0x1F)));
}

void CPU::op_nor()
{
    set_reg(instr.rd(), ~(regs[instr.rs()] | regs[instr.rt()]));
}

void CPU::op_lh() {
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    uint32_t value = (uint32_t)(short)bus->load16(addr);
    load = std::make_tuple(instr.rt(), value);
}

void CPU::op_sllv()
{
    set_reg(instr.rd(), regs[instr.rt()] << (int)(regs[instr.rs()] & 0x1F));
}

void CPU::op_lhu()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();
    if ((addr % 2) == 0) {
        uint32_t value = bus->load16(addr);
        load = std::make_tuple(instr.rt(), value);
    }
    else {
        exception(Exception::LoadAddressError);
    }
}

void CPU::op_rfe()
{
    uint32_t mode = sr & 0x3F;

    /* Shift kernel/user mode bits back. */
    sr &= ~(uint32_t)0xF;
    sr |= mode >> 2;
}

void CPU::op_mthi()
{
    hi = regs[instr.rs()];
}

void CPU::op_mtlo()
{
    lo = regs[instr.rs()];
}

void CPU::op_syscall()
{
    exception(Exception::SysCall);
}

void CPU::op_slt()
{
    bool condition = (int)regs[instr.rs()] < (int)regs[instr.rt()];
    set_reg(instr.rd(), condition ? 1u : 0u);
}

void CPU::op_mfhi()
{
    set_reg(instr.rd(), hi);
}

void CPU::op_divu()
{
    uint32_t n = regs[instr.rs()];
    uint32_t d = regs[instr.rt()];

    if (d == 0) {
        hi = n;
        lo = 0xFFFFFFFF;
    }
    else {
        hi = n % d;
        lo = n / d;
    }
}

void CPU::op_sltiu()
{
    bool condition = regs[instr.rs()] < instr.imm_s();
    set_reg(instr.rt(), condition ? 1u : 0u);
}

void CPU::op_srl()
{
    set_reg(instr.rd(), regs[instr.rt()] >> (int)instr.sa());
}

void CPU::op_mflo()
{
    set_reg(instr.rd(), lo);
}

void CPU::op_div()
{
    int n = (int)regs[instr.rs()];
    int d = (int)regs[instr.rt()];

    if (d == 0) {
        hi = (uint32_t)n;
        if (n >= 0) {
            lo = 0xFFFFFFFF;
        }
        else {
            lo = 1;
        }
    }
    else if ((uint32_t)n == 0x80000000 && d == -1) {
        hi = 0;
        lo = 0x80000000;
    }
    else {
        hi = (uint32_t)(n % d);
        lo = (uint32_t)(n / d);
    }
}

void CPU::op_sra()
{
    set_reg(instr.rd(), (uint32_t)((int)regs[instr.rt()] >> (int)instr.sa()));
}

void CPU::op_subu()
{
    set_reg(instr.rd(), regs[instr.rs()] - regs[instr.rt()]);
}

void CPU::op_slti()
{
    bool condition = (int)regs[instr.rs()] < (int)instr.imm_s();
    set_reg(instr.rt(), condition ? 1u : 0u);
}

void CPU::branch()
{
    next_pc = pc + (instr.imm_s() << 2);
    brancha = true;
}

void CPU::op_jalr()
{
    set_reg(instr.rd(), next_pc);
    op_jr();
}

void CPU::op_lbu()
{
    uint32_t value = bus->load8(regs[instr.rs()] + instr.imm_s());
    load = std::make_tuple(instr.rt(), value);
}

void CPU::op_blez()
{
    brancha = true;
    if (((int)regs[instr.rs()]) <= 0) {
        branch();
    }
}

void CPU::op_bgtz()
{
    brancha = true;
    if (((int)regs[instr.rs()]) > 0) {
        branch();
    }
}

void CPU::op_add()
{
    uint32_t rs = regs[instr.rs()];
    uint32_t rt = regs[instr.rt()];
    uint32_t add = rs + rt;

    bool overflow = add_overflow(rs, rt, add);
    if (!overflow)
        set_reg(instr.rd(), add);
    else
        exception(Exception::Overflow);
}

void CPU::op_and()
{
    set_reg(instr.rd(), regs[instr.rs()] & regs[instr.rt()]);
}

void CPU::op_mfc0() {
    uint32_t cpu_r = instr.rt();
    uint32_t cop_r = instr.rd();

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

void CPU::op_lwc0() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_lwc1() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_lwc2() {
    // Handle the GTE LWC instruction. You can customize this part.
    // If not supported, you can throw an exception like in the Rust code.
    // For example:
    throw std::runtime_error("Unhandled GTE LWC: " + std::to_string(instr.instruction));
}

void CPU::op_lwc3() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc0() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc1() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_swc2() {
    // Handle the GTE SWC instruction. You can customize this part.
    // If not supported, you can throw an exception like in the Rust code.
    // For example:
    throw std::runtime_error("Unhandled GTE SWC: " + std::to_string(instr.instruction));
}

void CPU::op_swc3() {
    // Not supported by this coprocessor
    exception(Exception::CoprocessorError);
}

void CPU::op_beq()
{
    brancha = true;
    if (regs[instr.rs()] == regs[instr.rt()]) {
        branch();
    }
}

void CPU::op_lb()
{
    uint32_t value = (uint32_t)(int8_t)bus->load8(regs[instr.rs()] + instr.imm_s());
    load = std::make_tuple(instr.rt(), value);
}

void CPU::op_jr()
{
    brancha = true;
    next_pc = regs[instr.rs()];
}

void CPU::op_sb()
{
    bus->store8(regs[instr.rs()] + instr.imm_s(), (uint8_t)regs[instr.rt()]);
}

void CPU::op_andi()
{
    set_reg(instr.rt(), regs[instr.rs()] & instr.imm());
}

void CPU::op_jal()
{
    set_reg(31, next_pc);
    op_j();
}

void CPU::op_sh()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();

    if ((addr % 4) == 0) {
        bus->store16(addr, (uint16_t)regs[instr.rt()]);
    }
    else {
        exception(Exception::StoreAddressError);
    }
}

void CPU::op_addu()
{
    set_reg(instr.rd(), regs[instr.rs()] + regs[instr.rt()]);
}

void CPU::op_sltu()
{
    bool condition = regs[instr.rs()] < regs[instr.rt()];
    set_reg(instr.rd(), condition ? 1u : 0u);
}

void CPU::op_lw()
{
    uint32_t addr = regs[instr.rs()] + instr.imm_s();

    if ((addr & 0x3) == 0) {
        uint32_t value = bus->load32(addr);
        load = std::make_tuple(instr.rt(), value);
    }
    else {
        exception(Exception::LoadAddressError);
    }
}

void CPU::op_addi()
{
    uint32_t rs = regs[instr.rs()];
    uint32_t imm_s = instr.imm_s();
    uint32_t addi = rs + imm_s;

    bool overflow = add_overflow(rs, imm_s, addi);
    if (!overflow)
        set_reg(instr.rt(), addi);
    else
        exception(Exception::Overflow);
}
// TODO
void CPU::op_bne()
{
    uint32_t rs = instr.rs();
    uint32_t rt = instr.rt();

    brancha = true;
    if (regs[rs] != regs[rt]) {
        branch();
    }
}

void CPU::op_mtc0() {
    uint32_t cpu_r = instr.rt();
    uint32_t cop_r = instr.rd();

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

void CPU::op_or()
{
    set_reg(instr.rd(), regs[instr.rs()] | regs[instr.rt()]);
}

void CPU::op_j()
{
    brancha = true;
    next_pc = (next_pc & 0xF0000000) | (instr.addr() << 2);
}

void CPU::op_addiu()
{
    uint32_t rt = instr.rt();
    uint32_t rs = instr.rs();
    uint32_t imm = instr.imm_s();

    set_reg(instr.rt(), regs[instr.rs()] + instr.imm_s());
}

void CPU::op_sll()
{
    uint32_t rd = instr.rd();
    uint32_t rt = instr.rt();
    uint32_t sa = instr.sa();

    set_reg(instr.rd(), regs[instr.rt()] << (int)instr.sa());
}

void CPU::op_sw()
{
    uint32_t r = instr.rs();
    uint32_t i = instr.imm_s();
    uint32_t addr = regs[r] + i;

    if ((addr & 0x3) == 0) {
        bus->store32(addr, regs[instr.rt()]);
    }
    else {
        exception(Exception::StoreAddressError);
    }
}

void CPU::op_lui()
{
    set_reg(instr.rt(), instr.imm() << 16);
}

void CPU::op_ori()
{
    set_reg(instr.rt(), regs[instr.rs()] | instr.imm());
}

void CPU::op_cop0() {
    switch (instr.rs()) {
    case (0b00100):
        op_mtc0();
        std::cout << "[CPU] INFO: MTC0 (COP0-Type)\n";
        break;

    case (0b00000):
        op_mfc0();
        std::cout << "[CPU] INFO: MFC0 (COP0-Type)\n";
        break;

    case (0b10000):
        op_rfe();
        std::cout << "[CPU] INFO: RFE (COP0-Type)\n";
        break;

    default:
        throw std::runtime_error("[CPU] ERROR: Unhandled COP0 Instruction");
        break;
    }
}

void CPU::op_cop1() {
    exception(Exception::CoprocessorError);
}

void CPU::op_cop3() {
    exception(Exception::CoprocessorError);
}

void CPU::op_cop2() {
    std::cout << "Unhandled GTE instruction: {" << std::to_string(instr.instruction);
}