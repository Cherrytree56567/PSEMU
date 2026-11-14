// Runs the emulated R3000 CPU on a raw little-endian MIPS binary, without a BIOS.
//
//   r3000_test [options] program.bin
//     --load ADDR          address to load the binary at and start executing from
//                          (default 0x80010000, KSEG0 mapping of RAM offset 0x10000)
//     --end ADDR           stop when the next instruction to fetch is at ADDR
//                          (default: the address just past the end of the binary)
//     --max N              fail if more than N instructions run (default 100000)
//     --expect REG=VALUE   after the run, REG (zero, t0, s1, r17, hi, lo, ...) must equal VALUE
//     --expect-mem A=VALUE after the run, the 32-bit word at address A must equal VALUE
//     --verbose            keep the CPU's own per-instruction logging
//
// Exit status: 0 all expectations hold, 1 an expectation failed or the run did not
// finish cleanly (CPU exception, bus error, instruction limit), 2 bad usage.

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "../Bus.h"
#include "../CPU.h"

namespace {

const char* const reg_names[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
};

struct RegExpect { std::string name; uint32_t value; };
struct MemExpect { uint32_t addr; uint32_t value; };

uint32_t parse_u32(const std::string& text) {
    size_t used = 0;
    long long v = std::stoll(text, &used, 0);
    if (used != text.size()) throw std::invalid_argument("bad number '" + text + "'");
    return static_cast<uint32_t>(v);
}

// Returns the CPU register value for a name, or throws if the name is unknown.
uint32_t read_reg(const CPU& cpu, const std::string& name) {
    if (name == "hi") return cpu.hi;
    if (name == "lo") return cpu.lo;
    for (uint32_t i = 0; i < 32; i++) {
        if (name == reg_names[i]) return cpu.regs[i];
    }
    if (name.size() > 1 && name[0] == 'r') {
        uint32_t i = parse_u32(name.substr(1));
        if (i < 32) return cpu.regs[i];
    }
    throw std::invalid_argument("unknown register '" + name + "'");
}

int usage() {
    std::cerr << "usage: r3000_test [--load ADDR] [--end ADDR] [--max N] [--verbose]\n"
                 "                  [--expect REG=VALUE]... [--expect-mem ADDR=VALUE]... program.bin\n";
    return 2;
}

bool split_pair(const std::string& arg, std::string& key, std::string& value) {
    size_t eq = arg.find('=');
    if (eq == std::string::npos || eq == 0 || eq + 1 == arg.size()) return false;
    key = arg.substr(0, eq);
    value = arg.substr(eq + 1);
    return true;
}

std::string hex32(uint32_t v) {
    char buf[16];
    snprintf(buf, sizeof buf, "0x%08x", v);
    return buf;
}

} // namespace

int main(int argc, char** argv) {
    uint32_t load_addr = 0x80010000;
    bool have_end = false;
    uint32_t end_addr = 0;
    uint64_t max_instructions = 100000;
    bool verbose = false;
    std::vector<RegExpect> reg_expects;
    std::vector<MemExpect> mem_expects;
    std::string path;

    try {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            std::string key, value;
            auto need_value = [&]() -> std::string {
                if (i + 1 >= argc) throw std::invalid_argument("missing value after " + arg);
                return argv[++i];
            };
            if (arg == "--load") load_addr = parse_u32(need_value());
            else if (arg == "--end") { end_addr = parse_u32(need_value()); have_end = true; }
            else if (arg == "--max") max_instructions = std::stoull(need_value(), nullptr, 0);
            else if (arg == "--verbose") verbose = true;
            else if (arg == "--expect") {
                if (!split_pair(need_value(), key, value)) throw std::invalid_argument("--expect wants REG=VALUE");
                reg_expects.push_back({key, parse_u32(value)});
            }
            else if (arg == "--expect-mem") {
                if (!split_pair(need_value(), key, value)) throw std::invalid_argument("--expect-mem wants ADDR=VALUE");
                mem_expects.push_back({parse_u32(key), parse_u32(value)});
            }
            else if (!arg.empty() && arg[0] == '-') throw std::invalid_argument("unknown option " + arg);
            else if (path.empty()) path = arg;
            else throw std::invalid_argument("more than one program given");
        }
        if (path.empty()) return usage();
    } catch (const std::exception& e) {
        std::cerr << "r3000_test: " << e.what() << "\n";
        return usage();
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "r3000_test: cannot open " << path << "\n";
        return 2;
    }
    std::vector<char> image((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (image.empty() || image.size() % 4 != 0) {
        std::cerr << "r3000_test: " << path << " must be a non-empty whole number of 32-bit words\n";
        return 2;
    }
    if (!have_end) end_addr = load_addr + static_cast<uint32_t>(image.size());

    // No BIOS is loaded: the program runs straight out of RAM.
    Bus bus;
    for (size_t i = 0; i < image.size(); i += 4) {
        uint32_t word = static_cast<uint8_t>(image[i]) |
                        (static_cast<uint32_t>(static_cast<uint8_t>(image[i + 1])) << 8) |
                        (static_cast<uint32_t>(static_cast<uint8_t>(image[i + 2])) << 16) |
                        (static_cast<uint32_t>(static_cast<uint8_t>(image[i + 3])) << 24);
        bus.store32(load_addr + static_cast<uint32_t>(i), word);
    }

    CPU cpu(&bus);
    cpu.reset();
    cpu.pc = load_addr;
    cpu.next_pc = load_addr + 4;

    // The CPU logs every instruction to stdout; keep that out of the way unless asked.
    std::streambuf* saved_cout = std::cout.rdbuf();
    std::ofstream null_out;
    if (!verbose) {
        null_out.open("/dev/null");
        if (null_out) std::cout.rdbuf(null_out.rdbuf());
    }

    uint64_t executed = 0;
    std::string run_error;
    try {
        while (cpu.pc != end_addr) {
            if (executed == max_instructions) {
                run_error = "instruction limit of " + std::to_string(max_instructions) +
                            " reached before pc hit the end address";
                break;
            }
            cpu.tick();
            executed++;
            if (cpu.cause != 0) {
                run_error = "CPU exception, cause code " + std::to_string((cpu.cause >> 2) & 0x1f) +
                            " at pc " + hex32(cpu.epc);
                break;
            }
        }
    } catch (const std::exception& e) {
        run_error = e.what();
    }
    std::cout.rdbuf(saved_cout);

    int failures = 0;
    if (!run_error.empty()) {
        std::cout << "FAIL run: " << run_error << "\n";
        failures++;
    }
    std::cout << "ran " << executed << " instructions, final pc " << hex32(cpu.pc) << "\n";

    for (const auto& e : reg_expects) {
        uint32_t actual = 0;
        try {
            actual = read_reg(cpu, e.name);
        } catch (const std::exception& ex) {
            std::cout << "FAIL " << ex.what() << "\n";
            failures++;
            continue;
        }
        bool ok = actual == e.value;
        std::cout << (ok ? "ok   " : "FAIL ") << e.name << ": expected " << hex32(e.value)
                  << ", got " << hex32(actual) << "\n";
        if (!ok) failures++;
    }
    for (const auto& e : mem_expects) {
        uint32_t actual = 0;
        try {
            actual = bus.load32(e.addr);
        } catch (const std::exception& ex) {
            std::cout << "FAIL mem " << hex32(e.addr) << ": " << ex.what() << "\n";
            failures++;
            continue;
        }
        bool ok = actual == e.value;
        std::cout << (ok ? "ok   " : "FAIL ") << "mem[" << hex32(e.addr) << "]: expected " << hex32(e.value)
                  << ", got " << hex32(actual) << "\n";
        if (!ok) failures++;
    }

    std::cout << (failures == 0 ? "PASS" : "FAILED") << " (" << failures << " failure"
              << (failures == 1 ? "" : "s") << ")\n";
    return failures == 0 ? 0 : 1;
}
