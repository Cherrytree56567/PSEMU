#include "Bios.h"

void Bios::new(const char* path) {
  std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return 1;
    }

    // Determine the file's size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Create a vector to store the data as uint8_t
    std::vector<uint8_t> data(file_size);

    // Read the file into the vector
    file.read(reinterpret_cast<char*>(data.data()), file_size);

    if (!file) {
        std::cerr << "[Bios] Error: Failed to read the file." << std::endl;
    }
    bdata.data = data;
}