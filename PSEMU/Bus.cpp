#include "Bus.h"

uint32_t Bus::mask_region(uint32_t addr) {
    // Index address space in 512MB chunks
    uint32_t index = (addr >> 29);

    return (addr & region_mask[index]);
}

// Load and Store functions
uint32_t Bus::load32(uint32_t addr) {

    if (addr % 4 != 0) {
        throw std::runtime_error("[Bus] ERROR: Unaligned load32 address " + std::to_string(addr));
    }

    uint32_t abs_addr = mask_region(addr);
    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        return ram.load32(offset.value());
    }
    else if (auto offset = BIOS.contains(abs_addr); offset.has_value()) {
        return bios.load32(offset.value());
    }
    else if (auto offset = IRQ_CONTROL.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: IRQ CONTROL NOT IMPLEMENTED. IRQ control read " << std::to_string(offset.value()) << "\n";
        return 0;
    }
    else if (auto offset = _DMA.contains(abs_addr); offset.has_value()) {
        //dma_reg(offset.value());
    }
    else if (auto offset = GPU.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: GPU NOT IMPLEMENTED. GPU read " << std::to_string(offset.value()) << "\n";
        switch (offset.value()) {
        case 4:
            return 0x1c000000;
            break;
        default:
            return 0;
            break;
        }
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled fetch32 at address " + std::to_string(addr));
}

void Bus::store32(uint32_t addr, uint32_t value) {
    uint32_t abs_addr = mask_region(addr);
    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        ram.store32(offset.value(), value);
        return;
    }
    else if (auto offset = BIOS.contains(abs_addr); offset.has_value()) {
        bios.store32(offset.value(), value);
        return;
    }
    else if (auto offset = IRQ_CONTROL.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: IRQ CONTROL NOT IMPLEMENTED. IRQ control: " << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
        return;
    }
    else if (auto offset = _DMA.contains(abs_addr); offset.has_value()) {
        //set_dma_reg(offset.value(), value);
        return;
    }
    else if (auto offset = GPU.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: GPU NOT IMPLEMENTED. GPU write " << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
        return;
    }
    else if (auto offset = TIMERS.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: Timer NOT IMPLEMENTED. Timer write register" << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
        return;
    }
    else if (auto offset = CACHE_CONTROL.contains(abs_addr); offset.has_value()) {
        std::cout << "[Bus] WARNING: Cache Control not implemented. Cache Control read " << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
        return;
    }
    else if (auto offset = MEM_CONTROL.contains(abs_addr); offset.has_value()) {
        switch (offset.value()) {
        case 0:
            if (value != 0x1f000000) {
                std::cout << "[Bus] ERROR: Bad Expansion 1 base address: " << std::to_string(value) << "\n";
            }
            break;
        case 4:
            if (value != 0x1f802000) {
                std::cout << "[Bus] ERROR: Bad Expansion 2 base address: " << std::to_string(value) << "\n";
            }
            break;
        default:
            std::cout << "[Bus] ERROR: Unhandled write to MEM Control " << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
            break;
        }
        return;
    }
    else if (auto offset = RAM_SIZE.contains(abs_addr); offset.has_value()) {
        return;
    }

    if (addr % 4 != 0) {
        throw std::runtime_error("[Bus] ERROR: Unaligned store32 address " + std::to_string(addr));
        return;
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled store32 into address " + std::to_string(addr));
}

void Bus::store16(uint32_t addr, uint16_t value) {
    uint32_t abs_addr = mask_region(addr);
    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        ram.store16(offset.value(), value);
        return;
    }
    else if (auto offset = SPU.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: SPU NOT IMPLEMENTED. SPU write register " << std::to_string(offset.value()) << "\n";
        return;
    }
    else if (auto offset = TIMERS.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: TIMER NOT IMPLEMENTED. Timer write register " << std::to_string(offset.value()) << "\n";
        return;
    }
    else if (auto offset = IRQ_CONTROL.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: IRQ CONTROL NOT IMPLEMENTED. IRQ control write " << std::to_string(offset.value()) << " " << std::to_string(value) << "\n";
        return;
    }

    if (addr % 4 != 0) {
        throw std::runtime_error("[Bus] ERROR: Unaligned store16 address " + std::to_string(addr));
        return;
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled store16 into address " + std::to_string(addr));
}

void Bus::store8(uint32_t addr, uint8_t value) {
    uint32_t abs_addr = mask_region(addr);
    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        ram.store8(offset.value(), value);
        return;
    }
    else if (auto offset = EXPANSION_2.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: Expansion 2 NOT IMPLEMENTED. Expansion 2 read register " << std::to_string(offset.value()) << "\n";
        return;
    }

    if (addr % 4 != 0) {
        throw std::runtime_error("[Bus] ERROR: Unaligned store8 address " + std::to_string(addr));
        return;
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled store8 into address " + std::to_string(addr));
}

uint8_t Bus::load8(uint32_t addr) {
    uint32_t abs_addr = mask_region(addr);

    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        return ram.load8(offset.value());
    }
    else if (auto offset = BIOS.contains(abs_addr); offset.has_value()) {
        return bios.load8(offset.value());
    }
    else if (auto offset = EXPANSION_1.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: Expansion 1 NOT IMPLEMENTED. Expansion 1 read register " << std::to_string(offset.value()) << "\n";
        return 0;
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled load8 into address " + std::to_string(addr));
}

uint16_t Bus::load16(uint8_t addr) {
    uint32_t abs_addr = mask_region(addr);

    if (auto offset = RAM_.contains(abs_addr); offset.has_value()) {
        return ram.load16(offset.value());
    }
    else if (auto offset = SPU.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: SPU NOT IMPLEMENTED. SPU read register " << std::to_string(offset.value()) << "\n";
        return 0;
    }
    else if (auto offset = IRQ_CONTROL.contains(abs_addr); offset.has_value()) {
        std::cout << "[BUS] WARNING: IRQ CONTROL NOT IMPLEMENTED. IRQ control read " << std::to_string(offset.value()) << "\n";
        return 0;
    }

    throw std::runtime_error("[Bus] ERROR: Unhandled load16 into address " + std::to_string(addr));
}

uint32_t Bus::dma_reg(uint32_t offset) {
    auto major = (offset & 0x70) >> 4;
    auto minor = offset & 0xf;
    auto channel = dma.channels[dma.from_index(major)];
    switch (major) {
    case 0:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 1:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 2:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 3:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 4:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 5:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 6:
        switch (minor) {
        case 8:
            return channel.control();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }

    case 7:
        switch (minor) {
        case 0:
            return dma.get_control();

        case 4:
            return dma.interrupt();

        default:
            std::cout << "[BUS] ERROR: Unhandled DMA read at " << std::to_string(offset) << "\n";
            exit(0);
        }
    default:
        std::cout << "[BUS] ERROR: Unhandled DMA access\n";
        exit(0);
        break;
    }
}

void Bus::set_dma_reg(uint32_t offset, uint32_t value) {
    switch (offset) {
    case 0x70:
        dma.set_control(value);
        break;

    default:
        std::cout << "[BUS] ERROR: Unhandled DMA write access\n";
        break;
    }
}