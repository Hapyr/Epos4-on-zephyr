# Epos4 on Zephyr

This project enables the control of one or more EPOS4 motor controllers from Maxon via CAN. 

The control can be done via a microcontroller with CAN interface. In the current version the libary is only implemented for Zephyr. In the past the project was also run on an Arduino. Please have a look at the first release. Zephyr was run on a STM32F407G-DISC1 with an external CAN transiever. 
For an easier handling with CAN a C++ class was implemented, which provides different functions to communicate with the EPOS4. 

For copyright reasons there is no public detailed documentation about the project. Most of the code is commented inline. Especially the examples. 
Since the lib was developed for Zephyr it is recommended to have some experience with it. Furthermore you should be familiar with the CAN interface of EPOS4. The documents added here can help. 

## General
Zephyr version 3.1.0 was used in the project. A Zephyr installation is required to use the code.

For a specific version you can call `west init` as follows:
```
west init -m https://github.com/zephyrproject-rtos/zephyr --mr zephyr-v3.1.0 <ZEPHYR_PROJECT_PATH>
```

The Epos4 works in different operating modes. Of these, an interface has been implemented for "profile position mode" (PPM), "cyclic synchronous position" (CSP), "cyclic synchronous velocity" (CSV) and "cyclic synchronous torque" (CST). Check the `epos4.h` for more information. 

There are 3 different applications in Examples. 

1. **epos_basic** is an example application where the EPOS is operated in CST mode. It receives a predefined sequence of torques.

2. **epos_matlab** is also an example application in CST mode. Instead of an arbitrary sequence, the EPOS is operated here in a closed control loop. The manipulated variables are calculated using a model that was created in Simulink and generated with the Simulink code to C code.

3. **epos_parameter_identification** is also an example application in CST mode. Here a torque trajectory is specified. The initial position or speed is measured and all relevant data is sent via the UART interface. This way the hardware can be identified.

**epos_matlab** is described in more detail below:
Zephyr is an open-source real-time operating system developed by the Linux Foundation for the IoT domain.  More information and a detailed documentation can be found at: https://docs.zephyrproject.org/latest/ For an easy start into the development I recommend to reprogram an introductory tutorial from the docs.

Since Zephyr uses cmake, the compiling information is specified via the [CMakeLists.txt](Examples/epos_matlab/CMakeLists.txt). There, among other things, the EPOS4 library and all C-generated files from Simulink are made known to the compiler. 
Simulink generates the files depending on the name of the respective subsystem. If the CMakeLists is not to be processed, care must be taken to name the corresponding subsystem in Simulink in the same way.

The Simulink subsystem was compiled in C using the Simulink coder. It is also possible to compile in C++, but then Simulink uses classes and the sample code must be adapted accordingly. 
In general, please note that logging for code generation must be switched off (MAT file logging). The STM32 cannot write to files by default. 
When Simulink generates C code, be sure to set the configuration `CONFIG_CPLUSPLUS=y` in the [prj.conf](epos4_zephyr/epos_conrtoller/prj.conf) and to include the libary as follows to avoid the include c++ mangle:
```c++
#ifdef __cplusplus
extern "C"
{
#endif
#include "Atomic.h"
#ifdef __cplusplus
}
#endif
```

The CAN interface of the STM32 was managed by the [stm32f4_disco.overlay](epos4_zephyr/epos_conrtoller/stm32f4_disco.overlay) devicetree extension.

# Collection of important commands
## Compile and flash
First, `west` and `zephyr` must be installed. Read about this at `https://docs.zephyrproject.org/latest/getting_started/index.html`.

    west build -b <bord_name> <project_folder>
    west flash
for certain changes, the >build< folder must be deleted before the build. You can read more about this in the zephyr docs.
So far the board `stm32f4_disco` was used in the project

## Plotter
The plotter makes it possible to display and evaluate the data sent via CAN to the microcontroller via uart on the PC. The data can be displayed graphically or written to files. This allows e.g. a parameter identification. 

### Virtual environment
If necessary, the virtual environment must be started first. 

    source venv/bin/activate
then the plotter can be started with:

    python3 plotter.py

## Device trees
Information about the board and its hardware can be changed and learned via the Zephyr device trees. 
For CAN there is a device tree overlay in the respective project folder. 
The general device tree can be found under `zephyr/boards/arm/stm32f4_disco/stm32f4_disco.dts`.

## General information about >epos_controller<

The adjustment of the frequency in the three cyclic synchronous real time modes of the epos4, the "Interpolation time period value" must be adjusted in the respective init functions (epos4::initCSP,initCSV,initCST). For more information about the "Interpolation time period value" read the `epos4 firmware spec. 6.2.121.1`

Also the interrupt frequency for sending the sync frame must be adjusted to the same time. For interrupts the Zephyr driver API was used and a timer object was used. (e.g. pos_controller.cpp:177). The interrupt time can be specified here in milliseconds and must be the same value as in the corresponding init function. Note the difference between hexadecimal and decimal.

# Note
If you have any questions, criticism, suggestions or ideas for improvement, please feel free to contact us. A more detailed code documentation will also be written to simplify the use. 
The code was developed as part of a project at Medit - RWTH Aachen University. Some content cannot be presented here for legal reasons. For more information, please contact jakob.gebler@rwth-aachen.de. 
The use is on own guarantee, I take over no responsibility with the use of the Libary.