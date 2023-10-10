cmake_minimum_required(VERSION 3.26)
project(PSEMU)

set(CMAKE_CXX_STANDARD 23)

add_executable(PSEMU
        Coprocessor.cpp
        CPU.cpp
        CPURegisters.cpp
        Logging.cpp
        Memory.cpp
        PSEMU.cpp
        VRAM.cpp
        VRAM.h
        GTE.cpp
        GTE.h
        GTE.h
        GPU.h
        gp0.cpp
        gp1.cpp
        GPU.cpp
        DMA.cpp
        DMA.h)
