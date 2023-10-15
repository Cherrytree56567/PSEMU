/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include <iostream>
#include "CPU.h"
#include "Bus.h"

int main() {
    Bus bus(1);
    bus.bios.newl("scph1001.bin");
    CPU cpu(&bus);
    while (true) {
      cpu.tick();
    }
    return 0;
}
