cmake_minimum_required(VERSION 3.16)
project(Sandwich_Roulette)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Sandwich_Roulette
    src/main.cpp
    src/Fs_Interface.cpp
    src/Menu.cpp
    src/Sandwich.cpp
    src/Roulette.cpp
)