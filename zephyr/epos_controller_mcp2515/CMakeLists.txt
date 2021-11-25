# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(epos_controller)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
#target_sources(app PRIVATE src/test.cpp)
target_sources(app PRIVATE src/main.cpp)
target_sources(app PRIVATE src/epos4/epos4.cpp)