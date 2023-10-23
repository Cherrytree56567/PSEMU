#pragma once
#include <cstdint>
#include <string>
#include <iostream>

enum Direction {
	ToRam = 0,
	FromRam = 1
};

/// DMA transfer step
enum Step {
	Increment = 0,
	Decrement = 1
};

/// DMA transfer synchronization mode
enum Sync {
	/// Transfer starts when the CPU writes to the Trigger bit and
	/// transfers everything at once
	Manual = 0,
	/// Sync blocks to DMA requests
	Request = 1,
	/// Used to transfer GPU command lists
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
	}

	uint32_t control();
	void set_control(uint32_t val);

private:
	bool enable;
	Direction direction;
	Step step;
	Sync sync;
	bool trigger;
	bool chop;
	uint8_t chop_dma_sz;
	uint8_t chop_cpu_sz;
	uint8_t dummy;
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

private:
	uint32_t control;
	bool irq_en;
	uint8_t channel_irq_en;
	uint8_t channel_irq_flags;
	bool force_irq;
	uint8_t irq_dummy;
};

