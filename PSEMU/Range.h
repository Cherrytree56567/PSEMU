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

const Range _BIOS = Range(0xbfc00000, 512 * 1024);
const Range MEM_CONTROL = Range(0x1f801000, 36);