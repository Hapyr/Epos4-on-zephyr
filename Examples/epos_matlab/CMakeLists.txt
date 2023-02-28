# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(epos_controller_mainTest)

target_sources(app PRIVATE 
    src/main.cpp
    ../../Library/epos4/epos4.cpp
    src/simulinkcode/controller/Control_System.c
    src/simulinkcode/controller/referenced_model_includes/PositionControl.c
    src/simulinkcode/controller/referenced_model_includes/VelocityControl.c
    src/simulinkcode/Matlab/R2018b/rtw/c/src/rt_logging.c
    src/simulinkcode/Matlab/_sharedutils/rt_nonfinite.c
    src/simulinkcode/Matlab/_sharedutils/rtGetInf.c
    src/simulinkcode/Matlab/_sharedutils/rtGetNaN.c
)

include_directories(../../Library/epos4)

target_include_directories(app PUBLIC src/simulinkcode/controller main.cpp)
target_include_directories(app PUBLIC src/simulinkcode/controller/referenced_model_includes main.cpp)
target_include_directories(app PUBLIC src/simulinkcode/Matlab/R2018b/extern/include main.cpp)
target_include_directories(app PUBLIC src/simulinkcode/Matlab/R2018b/simulink/include main.cpp)
target_include_directories(app PUBLIC src/simulinkcode/Matlab/R2018b/rtw/c/src main.cpp)
target_include_directories(app PUBLIC src/simulinkcode/Matlab/_sharedutils main.cpp)