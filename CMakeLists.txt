# Copyright (C) 2020  Igara Studio S.A.

cmake_minimum_required(VERSION 3.16)
project(fuzz)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(FATAL_ERROR "Clang is required")
endif()

add_definitions(-fsanitize=address,fuzzer)

include_directories(.)
add_subdirectory(flic)
add_subdirectory(tga)

add_executable(fuzz_flic fuzz_flic.cpp)
add_executable(fuzz_tga fuzz_tga.cpp)
target_link_libraries(fuzz_flic flic-lib)
target_link_libraries(fuzz_tga tga-lib)

target_link_options(fuzz_flic PUBLIC -fsanitize=address,fuzzer)
target_link_options(fuzz_tga PUBLIC -fsanitize=address,fuzzer)
