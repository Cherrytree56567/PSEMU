/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <climits>
#include <stddef.h>
#include "Logging.h"
#include "VRAM.h"

class GPU {
public:
	// [ For GPU Memory ]
	GPU(size_t gpmemsize) : gpuVRAM(gpmemsize) {}

    void RunComands();

private:
	VRAM gpuVRAM;
    // Command Counter
    int cc;
};
