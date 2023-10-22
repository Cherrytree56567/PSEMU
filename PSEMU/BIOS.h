/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#pragma once
#include <string>

class BIOS {
public:
	BIOS(std::string path);
	uint32_t load32(uint32_t offset);

private:
	uint8_t data[512 * 1024] = { 0x0 };
};

