cmake_minimum_required(VERSION 3.12)


project(
        hamrac
        VERSION 1.0
        DESCRIPTION "C++ Year 2022 Lab4"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(lib/hamming_code)
add_subdirectory(lib/huffman_code)
add_subdirectory(lib/archiver)
add_subdirectory(bin)