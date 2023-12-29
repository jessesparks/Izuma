cmake_minimum_required(VERSION 3.21)
project(IzumaEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/IzumaEngine/src)
link_directories(${CMAKE_BINARY_DIR}/IzumaEngine)

file(GLOB_RECURSE SOURCES src/**.h src/**.cpp)

add_executable(IzumaEditor ${SOURCES})
target_link_libraries(IzumaEditor IzumaEngine)