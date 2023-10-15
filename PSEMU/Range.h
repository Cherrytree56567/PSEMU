#pragma once
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <optional>

struct Range {
    uint32_t start;
    uint32_t length;

    Range(uint32_t s, uint32_t l) : start(s), length(l) {}

    inline bool contains(uint32_t addr) const {
        return (addr >= start && addr < start + length);
    }

    inline uint32_t offset(uint32_t addr) const {
        return addr - start;
    }
};
