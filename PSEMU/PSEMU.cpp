/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-ShareAlike 4.0 International

*/
#include <iostream>
#include "CPU.h"

int main() {
    CPU cpu("scph1001.bin");

    while (true) {
        cpu.tick();
    }
    return 0;
}
