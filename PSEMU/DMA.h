#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <optional>

enum Direction {
	ToRam = 0,
	FromRam = 1
};

// DMA transfer step
enum Step {
	Increment = 0,
	Decrement = 1
};

enum Port {
	/// Macroblock decoder input
	MdecIn = 0,
	/// Macroblock decoder output
	MdecOut = 1,
	/// Graphics Processing Unit
	Gpu = 2,
	/// CD-ROM drive
	CdRom = 3,
	/// Sound Processing Unit
	Spu = 4,
	/// Extension port
	Pio = 5,
	/// Used to clear the ordering table
	Otc = 6,
	NUL = 100
};

// DMA transfer synchronization mode
enum Sync {
	// Transfer starts when the CPU writes to the Trigger bit and
	// transfers everything at once
	Manual = 0,
	// Sync blocks to DMA requests
	Request = 1,
	// Used to transfer GPU command lists
	LinkedList = 2
};

class DMAChannel {
public:
	DMAChannel() {
		enable = false;
		direction = Direction::ToRam;
		step = Step::Increment;
		sync = Sync::Manual;
		trigger = false;
		chop = false;
		chop_dma_sz = 0;
		chop_cpu_sz = 0;
		dummy = 0;
		base = 0;
		block_size = 0;
		block_count = 0;
	}

	uint32_t control();
	bool active();
	void set_control(uint32_t val);
	std::optional<uint32_t> transfer_size();
	void done();
	uint32_t block_control();
	void set_block_control(uint32_t val);
	uint32_t get_base() { return base; };
	void set_base(uint32_t value) {
		base = value & 0xffffff;
	}
	bool enable;
	Direction direction;
	Step step;
	Sync sync;
	bool trigger;
	bool chop;
	uint8_t chop_dma_sz;
	uint8_t chop_cpu_sz;
	uint8_t dummy;
	uint32_t base;
	uint16_t block_size;
	uint16_t block_count;
};

class DMA {
public:
	DMA() {
		control = 0x07654321;
	}

	uint32_t get_control() { return control; }
	void set_control(uint32_t value) { control = value; }

	bool irq();
	uint8_t interrupt();
	void set_interrupt(uint32_t value);

	Port from_index(uint32_t index) {
		switch (index){
		case 0:
			return Port::MdecIn;

		case 1:
			return Port::MdecOut;

		case 2:
			return Port::Gpu;

		case 3:
			return Port::CdRom;

		case 4:
			return Port::Spu;

		case 5:
			return Port::Pio;

		case 6:
			return Port::Otc;

		default: 
			std::cout << "[DMA] ERROR: Invalid port " << std::to_string(index) << "\n";
			exit(0);
		}
	}
	uint32_t control;
	bool irq_en;
	uint8_t channel_irq_en;
	uint8_t channel_irq_flags;
	bool force_irq;
	uint8_t irq_dummy;
	DMAChannel channels[7];
};

