cmake_minimum_required(VERSION 3.21)
project(IzumaEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp)

add_library(IzumaEngine STATIC ${SOURCES})

target_compile_definitions(IzumaEngine PUBLIC IZ_ENABLE_ASSERTS)

target_include_directories(IzumaEngine PUBLIC ../vendor/GameNetworkingSockets/include)

if(${CMAKE_SYSTEM} MATCHES Windows)
    target_link_libraries(IzumaEngine LINK_PUBLIC ${PROJECT_SOURCE_DIR}/../vendor/GameNetworkingSockets/bin/GameNetworkingSockets.dll)
    message(STATUS "Windows Detected")
elseif(${CMAKE_SYSTEM} MATCHES Linux)
    target_link_libraries(IzumaEngine LINK_PUBLIC -lxcb LINK_PUBLIC -lX11-xcb LINK_PUBLIC -lX11)
    message(STATUS "Linux Detected")
endif()