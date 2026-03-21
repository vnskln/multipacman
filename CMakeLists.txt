cmake_minimum_required(VERSION 3.16)
project(MultiPacman)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)
add_executable(multipacman ${SOURCES})
