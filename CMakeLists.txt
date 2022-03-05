# Copyright (C) 2020-2022  Igara Studio S.A.

cmake_minimum_required(VERSION 3.16)
project(fuzz)

include_directories(.)
add_subdirectory(flic)
add_subdirectory(tga)

add_executable(fuzz_flic fuzz_flic.cpp)
add_executable(fuzz_tga fuzz_tga.cpp)
target_link_libraries(fuzz_flic flic-lib)
target_link_libraries(fuzz_tga tga-lib)

target_link_options(fuzz_flic PUBLIC -fsanitize=address,fuzzer)
target_link_options(fuzz_tga PUBLIC -fsanitize=address,fuzzer)

set(ASEPRITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../aseprite)
if(EXISTS ${ASEPRITE_DIR})
  set(ZLIB_LIBRARIES zlib)
  add_subdirectory(${ASEPRITE_DIR}/third_party/zlib zlib)
  add_subdirectory(${ASEPRITE_DIR}/laf laf)
  if(EXISTS ${ASEPRITE_DIR}/third_party/cityhash)
    add_subdirectory(${ASEPRITE_DIR}/third_party/cityhash cityhash)
  endif()
  add_subdirectory(${ASEPRITE_DIR}/third_party/fmt fmt)
  add_subdirectory(${ASEPRITE_DIR}/src/fixmath fixmath)
  add_subdirectory(${ASEPRITE_DIR}/src/doc doc)
  add_subdirectory(${ASEPRITE_DIR}/src/dio dio)

  add_executable(fuzz_ase fuzz_ase.cpp)
  target_link_libraries(fuzz_ase dio-lib)
  target_link_options(fuzz_ase PUBLIC -fsanitize=address,fuzzer)
endif()
