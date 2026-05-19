cmake_minimum_required(VERSION 3.6)

project(glad)

file(GLOB SRC_FILES "src/*.c")

add_library(glad STATIC ${SRC_FILES})

target_include_directories(glad
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
