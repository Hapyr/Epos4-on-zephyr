# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(epos_controller_mainTest)

target_sources(app PRIVATE 
    src/main.cpp
    ../../Library/epos4/epos4.cpp
)

include_directories(../../Library/epos4)