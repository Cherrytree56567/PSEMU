#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <optional>

class Range {
    uint32_t start;
    uint32_t length;

    Range(uint32_t s, uint32_t l) : start(s), length(l) {}

    std::optional<uint32_t> contains(uint32_t addr) {
        if (addr >= start && addr < start + length) {
            return addr - start;
        } else {
            return std::nullopt;
        }
    }
};