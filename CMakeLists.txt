cmake_minimum_required(VERSION 3.15)

project(InfineonMK_driver)
set(EX ${CMAKE_PROJECT_NAME})

add_executable(${EX} main.c)

add_subdirectory(TLE8242)
target_link_libraries(${EX} PRIVATE TLE8242)