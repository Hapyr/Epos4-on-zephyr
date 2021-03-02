# epos4-arduino-seeed

Libary for the Arduino (tested on the Arduino UNO 8 bit) with the CAN-BUS Shield V2.0 with the MCP2515. The attached mcp2515_can.h and mcp2515_can_dfs.h were used.

Implemented are different PDO and SDO send functions. For the state machine of the EPOS4 the epos4::init() was written, which queries the current state and switches on the EPOS4 accordingly.

For the "Profile Position Mode" (PPM), the "Cyclic Synchronous Position Mode" (CSP), the "Cyclic Synchronous Velocity Mode" (CSV) and the "Cyclic Synchronous Torque Mode" init functions were written and test functions were provided in the .ino file. Depending on the test function (or use of own functions) the corresponding mode must be initialized before.

When initializing the epos4 object the configPDO function is called which sets the TxPDOs and RxPDOs according to the variables in the epos4.h file.

If you have any questions, criticism, suggestions or ideas for improvement, please feel free to contact us. The libary is currently in full development and will be further improved. A more detailed code documentation will also be written to simplify the use. The use is on own guarantee, I take over no responsibility with the use of the Libary.
