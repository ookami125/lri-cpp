cmake_minimum_required(VERSION 3.10)

project(lri_extractor)

set (CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(lri_extractor
    src/main.cpp
    src/image.cpp
    src/debayer.cpp
    src/picoproto.cpp
)
