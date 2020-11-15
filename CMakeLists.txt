cmake_minimum_required(VERSION 3.0)
set(CMAKE_SKIP_RPATH TRUE)

project(CRaudio)

if(WIN32)
    link_directories(lib/win64)
else(WIN32)
    link_directories(lib/linux64)
endif(WIN32)

include_directories(include)

add_definitions("-Wall -pedantic")
if(NOT WIN32)
    add_definitions("-fPIC")
endif(NOT WIN32)

add_subdirectory(src)
