# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr)
project(epos_controller_model)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
target_sources(app PRIVATE src/main.c
    src/codegeneration/assdf_grt_rtw/assdf.c
    src/codegeneration/assdf_grt_rtw/assdf_data.c 
)

target_include_directories(app PUBLIC src/codegeneration/assdf_grt_rtw main.c)
target_include_directories(app PUBLIC src/R2021a/extern/include main.c)
target_include_directories(app PUBLIC src/R2021a/simulink/include main.c)