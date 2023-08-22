#include "Logging.h"

void Logging::err(std::string message) {
    system("Color 0A");
    std::cout << "ERROR: " << message;
    system("Color 7A");
}

void Logging::warn(std::string message) {
    system("Color EA");
    std::cout << "WARNING: " << message;
    system("Color 7A");
}

void Logging::info(std::string message) {
    std::cout << "INFO: " << message;
}