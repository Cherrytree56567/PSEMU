/*
 *************************************
 *           PSEMU Licence           *
 *************************************

 PSEMU © 2023 by Ronit D'silva is licensed under Attribution-NonCommercial-NoDerivatives 4.0 International

*/
#include "Logging.h"

void Logging::err(std::string message) {
    if (errstatus) {
        system("Color 0A");
        std::cout << "ERROR: " << message << std::endl;
        system("Color 7A");
    }
}

void Logging::warn(std::string message) {
    if (warnstatus) {
        system("Color EA");
        std::cout << "WARNING: " << message << std::endl;
        system("Color 7A");
    }
}

void Logging::log(std::string message) {
    if (infostatus) {
        std::cout << "INFO: " << message << std::endl;
    }
}