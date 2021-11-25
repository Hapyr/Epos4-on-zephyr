# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(epos_controller)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
# target_sources(app PRIVATE src/test.cpp)

target_sources(app PRIVATE 
    src/pos_controller.cpp
    src/epos4/epos4.cpp
    src/simulinkcode/Parameteridentification_example_lb/Atomic_grt_rtw/Atomic.c
    src/simulinkcode/Parameteridentification_example_lb/Atomic_grt_rtw/Atomic_data.c 
)

target_include_directories(app PUBLIC src/simulinkcode/Parameteridentification_example_lb/Atomic_grt_rtw pos_controller.cpp)
target_include_directories(app PUBLIC src/simulinkcode/R2021a/extern/include pos_controller.cpp)
target_include_directories(app PUBLIC src/simulinkcode/R2021a/rtw/c/src pos_controller.cpp)
target_include_directories(app PUBLIC src/simulinkcode/R2021a/simulink/include pos_controller.cpp)