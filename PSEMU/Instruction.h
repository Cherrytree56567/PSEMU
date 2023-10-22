#pragma once
#include <cstdint>

class Instruction {
public:
	uint32_t value;
	uint32_t function() { return value & 0x3F; }
	uint32_t opcode() { return value >> 26; }
	uint32_t imm() { return value & 0xFFFF; }
	uint32_t rt() { return(value >> 16) & 0x1F; }
	uint32_t rs() { return (value >> 21) & 0x1F; }
	uint32_t imm_s() { return(uint32_t)(int16_t)imm(); }
};