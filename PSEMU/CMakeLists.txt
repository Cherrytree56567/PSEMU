cmake_minimum_required(VERSION 3.26)
project(PSEMU)

set(CMAKE_CXX_STANDARD 23)

add_executable(PSEMU Coprocessor.cpp CPU.cpp CPURegisters.cpp GPU.cpp Logging.cpp Memory.cpp PSEMU.cpp)
