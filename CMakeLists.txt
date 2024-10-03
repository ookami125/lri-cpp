cmake_minimum_required(VERSION 3.10)

project(lri_extractor)

add_executable(lri_extractor
    src/main.cpp
    src/image.cpp
    src/debayer.cpp
    src/picoproto.cpp
)

set (CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

target_compile_options(lri_extractor PRIVATE -msse4.1)