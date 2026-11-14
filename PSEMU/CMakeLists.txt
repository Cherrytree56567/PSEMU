cmake_minimum_required(VERSION 3.26)
project(PSEMU)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PSEMU
        BIOS.cpp
        Bus.cpp
        CPU.cpp
        DMA.cpp
        PSEMU.cpp
        RAM.cpp
)

if(NOT MSVC)
    target_compile_options(PSEMU PRIVATE -Wall)
endif()
