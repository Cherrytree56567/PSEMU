#pragma once
#include <cstdint>

class DMA {
public:
	DMA() {
		control = 0x07654321;
	}

	uint32_t get_control() { return control; }

private:
	uint32_t control;
};

