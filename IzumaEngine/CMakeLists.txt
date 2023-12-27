cmake_minimum_required(VERSION 3.21)
project(IzumaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp)

add_library(IzumaEngine STATIC ${SOURCES})

target_compile_definitions(IzumaEngine PUBLIC IZ_ENABLE_ASSERTS)