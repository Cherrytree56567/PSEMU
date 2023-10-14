#include "Bios.h"

void Bios::newl(const char* path) {
  std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return 1;
    }

    // Determine the file's size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into the vector
    file.read(reinterpret_cast<char*>(data.data()), file_size);

    if (!file) {
        std::cerr << "[Bios] Error: Failed to read the file." << std::endl;
    }
}

uint32_t Bios::load32(uint32_t offset) {
  uint32_t b0 = static_cast<uint32_t>(data[offset + 0]);
    uint32_t b1 = static_cast<uint32_t>(data[offset + 1]);
    uint32_t b2 = static_cast<uint32_t>(data[offset + 2]);
    uint32_t b3 = static_cast<uint32_t>(data[offset + 3]);

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}