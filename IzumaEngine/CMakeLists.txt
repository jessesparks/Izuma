cmake_minimum_required(VERSION 3.21)
project(IzumaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp)

add_library(IzumaEngine STATIC ${SOURCES})

target_compile_definitions(IzumaEngine PUBLIC IZ_ENABLE_ASSERTS)

include_directories(../vendor/GameNetworkingSockets/include)

if(${CMAKE_SYSTEM_NAME} EQUAL Windows)
    target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/vendor/GameNetworkingSockets/bin/GameNetworkingSockets.dll)
    message("Windows Detected")
endif()

if(${CMAKE_SYSTEM_NAME} EQUAL Linux)
    target_link_libraries(IzumaEngine LINK_PUBLIC -lxcb LINK_PUBLIC -lX11-xcb LINK_PUBLIC -lX11)
    message("Linux Detected")
endif()