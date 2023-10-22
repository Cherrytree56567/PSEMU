/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <cstring>
#include <optional>

class Range {
public:
	Range(uint32_t st, uint32_t len) :
		start(st), length(len) {}

	std::optional<uint32_t> contains(uint32_t addr) const {
		if (addr >= start && addr < start + length)
			return (addr - start);
		else
			return std::nullopt;
	}

public:
	uint32_t offset = 0;

private:
	uint32_t start, length;
};

const Range _RAM = Range(0x00000000, 2048 * 1024);
const Range _BIOS = Range(0x1fc00000, 512 * 1024);
const Range SYS_CONTROL = Range(0x1f801000, 36);
const Range RAM_SIZE = Range(0x1f801060, 4);
const Range CACHE_CONTROL = Range(0xfffe0130, 4);
const Range SPU = Range(0x1f801c00, 640);
const Range EXPANSION_2 = Range(0x1f802000, 66);
const Range EXPANSION_1 = Range(0x1f000000, 512 * 1024);
const Range IRQ_CONTROL = Range(0x1f801070, 8);
const Range TIMERS = Range(0x1f801100, 0x30);
const Range DMA = Range(0x1f801080, 0x80);
const Range GPU_RANGE = Range(0x1f801810, 8);