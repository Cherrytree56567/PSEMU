#include "DMA.h"
#include "Bits.h"

bool DMA::irq() {
	auto channel_irq = channel_irq_flags & channel_irq_en;

	return force_irq || (irq_en && channel_irq != 0);
}

uint8_t DMA::interrupt() {
	auto r = 0;

	r |= (uint32_t)irq_dummy;
	r |= ((uint32_t)force_irq) << 15;
	r |= ((uint32_t)channel_irq_en) << 16;
	r |= ((uint32_t)irq_en) << 23;
	r |= ((uint32_t)channel_irq_flags) << 24;
	r |= ((uint32_t)irq()) << 31;

	return r;
}

void DMA::set_interrupt(uint32_t value) {
	// Unknown what bits [5:0] do
	irq_dummy = (uint8_t)(value & 0x3f);

	force_irq = ANY_BIT_SET(value, 1 << 15);

	channel_irq_en = (uint8_t)((value >> 16) & 0x7f);

	irq_en = ANY_BIT_SET(value, 1 << 23);

	// Writing 1 to a flag resets it
	auto ack = (uint8_t)((value >> 24) & 0x3f);
	channel_irq_flags &= !ack;
}

uint32_t DMAChannel::control() {
	auto r = 0;

	r |= ((uint32_t)direction) << 0;
	r |= ((uint32_t)step) << 1;
	r |= ((uint32_t)chop) << 8;
	r |= ((uint32_t)sync) << 9;
	r |= ((uint32_t)chop_dma_sz) << 16;
	r |= ((uint32_t)chop_cpu_sz) << 20;
	r |= ((uint32_t)enable) << 24;
	r |= ((uint32_t)trigger) << 28;
	r |= ((uint32_t)dummy) << 29;

	return r;
}

void DMAChannel::set_control(uint32_t val) {
	if (ANY_BIT_SET(val, 1))
		direction = Direction::FromRam;
	else
		direction = Direction::ToRam;

	if (ANY_BIT_SET(val, 2))
		step = Step::Decrement;
	else
		step = Step::Increment;

	chop = ANY_BIT_SET(val, 1 << 8);

	switch ((val >> 9) & 3) {
	case 0:
		sync = Sync::Manual;
		break;

	case 1:
		sync = Sync::Request;
		break;

	case 2:
		sync = Sync::LinkedList;
		break;

	default:
		std::cout << "[DMA] ERROR: Unknown DMA sync mode " << std::to_string((val >> 9) & 3) << "\n";
		break;
	};

	chop_dma_sz = (uint8_t)((val >> 16) & 7);
	chop_cpu_sz = (uint8_t)((val >> 20) & 7);

	enable = ANY_BIT_SET(val, 1 << 24);
	trigger = ANY_BIT_SET(val, 1 << 28);

	dummy = (uint8_t)((val >> 29) & 3);
}

uint32_t DMAChannel::block_control() {
	auto bs = (uint32_t)block_size;
	auto bc = (uint32_t)block_count;

	return (bc << 16) | bs;
}

/// Set value of the Block Control register
void DMAChannel::set_block_control(uint32_t val) {
	block_size = (uint16_t)val;
	block_count = (uint16_t)(val >> 16);
}

bool DMAChannel::active() {
	// In manual sync mode the CPU must set the "trigger" bit
	// to start the transfer.
	auto triggera = [&]() {
		switch (sync) {
		case Sync::Manual:
			return trigger;
		default:
			return true;
		}
		}();

	return enable&& triggera;
}

std::optional<uint32_t> DMAChannel::transfer_size() {
	auto bs = (uint32_t)block_size;
	auto bc = (uint32_t)block_count;

	switch (sync) {
	case Sync::Manual:
		return bs;

	case Sync::Request:
		return bc * bs;

	case Sync::LinkedList:
		return std::nullopt;
	}

	return 0;
}

void DMAChannel::done() {
	enable = false;
	trigger = false;

	// XXX Need to set the correct value for the other fields
	// (in particular interrupts)
}

