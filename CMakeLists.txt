cmake_minimum_required(VERSION 3.10)

project(tinyrenderer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    main.cpp
    tgaimage.h
)

add_executable(tinyrenderer ${SOURCES})

target_include_directories(tinyrenderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})