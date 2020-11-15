cmake_minimum_required(VERSION 3.0)

project(CRaudio)

option(BUILD_SHARED "Build CRaudio as a shared library" ON)

if(WIN32)
    link_directories(lib/win64)
else(WIN32)
    link_directories(lib/linux64)
endif(WIN32)

include_directories(include)
add_definitions("-Wall -pedantic")
add_subdirectory(src)