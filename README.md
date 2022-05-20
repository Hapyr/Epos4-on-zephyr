**Over the past six months, we have refined our documentation for intere purposes and made it more detailed. This will be published in the next few weeks. **

# epos4

This project allows the control of one or more EPOS4 motor controllers from Maxon via CAN. 

The control can be done via a microcontroller with CAN interface. In this project, an Arduino example (Arduino Uno and MEGA) and a Zephyr project were implemented. Zephyr was run on a STM32F407G-DISC1 with an external CAN transiever. 
For easier handling with CAN, a C++ class was implemented that provides various functions to communicate with the EPOS4. The class was adapted for Arduino and Zephyr. 

## arduino-seeed implementation

Libary for the Arduino (tested on the Arduino UNO 8 bit) with the CAN-BUS Shield V2.0 with the MCP2515. The attached mcp2515_can.h and mcp2515_can_dfs.h were used.

Implemented are different PDO and SDO send functions. For the state machine of the EPOS4 the epos4::init() was written, which queries the current state and switches on the EPOS4 accordingly.

For the "Profile Position Mode" (PPM), the "Cyclic Synchronous Position Mode" (CSP), the "Cyclic Synchronous Velocity Mode" (CSV) and the "Cyclic Synchronous Torque Mode" init functions were written and test functions were provided in the .ino file. Depending on the test function (or use of own functions) the corresponding mode must be initialized before.

When initializing the epos4 object the configPDO function is called which sets the TxPDOs and RxPDOs according to the variables in the epos4.h file.

In the example, note that no interraupts were used for the timing of the synchronisation except for the cst_test function. 

In general, the libary for the arduino contains the same functionality as the zephyr implementation. The only difference is the function to send a CAN frame. In a future generalisation, a call back function could be defined which implements this functionality outside the libary. 

## zephyr implementation

Generally, the same applies as described in the Arduino section. The implementation on zephyr is an extension.

There are 3 different applications in epos4_zephyr. 

1. **epos_controller** is the main application and shows exemplarily how a subsystem (named Atmoic) from Simulink (code generated in C without MAT file logging) can be integrated into a closed loop. An example without a subsystem of Simulink is given in main.cpp. This code shows how the library can be used to control the epos4 motor driver. There, however, no interrupt functionality was implemented but only the PPM mode was used (see epos4::moveToPosition). For the cyclic sync modes, see pos_controller.cpp to understand how interrupts were used for the timing of the sync. 

2. **epos_controller_mcp2515** Provides the same functionality. However, instead of the onboard CAN devices of the STM32F407G, it uses the Arduino Shield MCP2515 which is controlled via SPI. 

3. **epos_controller_model** only shows an integration of a Simulink subsystem in C. 

**epos_controller** is described in more detail below:
Zephyr is an open source real-time operating system developed by the Linux Foundation for the IoT domain. 
More information and detailed documentation can be found at https://docs.zephyrproject.org/latest/.
For an easy start into the development, I recommend reprogramming an introductory tutorial from the docs. 

Since Zephyr uses cmake, the compiling information is given in the [CMakeLists.txt](epos4_zephyr/epos_conrtoller/CMakeLists.txt). Among other things, the EPOS4 library and all C-generated files from Simulink are made known to the compiler there. 
Simulink generates the files depending on the name of the respective subsystem. If the CMakeLists are not to be edited, make sure that the corresponding subsystem is named the same in Simulink. 

The CAN interface of the STM32 was managed via the [stm32f4_disco.overlay](epos4_zephyr/epos_conrtoller/stm32f4_disco.overlay) Devicetree extension. 

Simulink libarys needed to run the simulink code generation are not available here. 

# collection of important commands
## compile and flash
First, `west` and `zephyr` must be installed. Read about this at `https://docs.zephyrproject.org/latest/getting_started/index.html`.

    west build -b <bord_name> <project_folder>
    west flash
for certain changes, the >build< folder must be deleted before the build. You can read more about this in the zephyr docs.
So far the board `stm32f4_disco` was used in the project

## Plotter with virtual environment
The plotter can be used to plot data that is sent from the controller to the pc via uart.
If necessary, the virtual environment must be started first. 

    source venv/bin/activate
then the plotter can be started with:

    python3 pythonploter2.py

## Device trees
Information about the board and its hardware can be changed and learned via the Zephyr device trees. 
For CAN there is a device tree overlay in the respective project folder. 
The general device tree can be found under `zephyr/boards/arm/stm32f4_disco/stm32f4_disco.dts`.

## General information about >epos_controller<

The adjustment of the frequency in the three cyclic synchronous real time modes of the epos4, the "Interpolation time period value" must be adjusted in the respective init functions (epos4::initCSP,initCSV,initCST). For more information about the "Interpolation time period value" read the `epos4 firmware spec. 6.2.121.1`

Also the interrupt frequency for sending the sync frame must be adjusted to the same time. For interrupts the Zephyr driver API was used and a timer object was used. (e.g. pos_controller.cpp:177). The interrupt time can be specified here in milliseconds and must be the same value as in the corresponding init function. Note the difference between hexadecimal and decimal.

# Note
If you have any questions, criticism, suggestions or ideas for improvement, please feel free to contact us. A more detailed code documentation will also be written to simplify the use. 
The code was developed as part of a project at RWTH Aachen University. Some content cannot be presented here for legal reasons. For more information, please contact jakob.gebler@rwth-aachen.de. 
The use is on own guarantee, I take over no responsibility with the use of the Libary.
