/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include "BIOS.h"
#include "Range.h"
#include <iostream>

class Bus {
public:
	Bus(std::string a) : bios(a) {}
	uint32_t load32(uint32_t address);

private:
	BIOS bios;
};

