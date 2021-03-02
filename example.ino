#include "Arduino.h"

#include "mcp2515_can.h"
#include "mcp2515_can_dfs.h"

#include <epos4.h>

#define MAX_DATA_SIZE 64
#define SPI_CS_PIN 9
#define CAN_INT_PIN 2

mcp2515_can CAN_INTERFACE(SPI_CS_PIN);
epos4 epos4_1(0x581,0x601, &CAN_INTERFACE);

float dt = 0.0;
unsigned long startTime;
unsigned long endTime;


void setup()
{
    Serial.begin(115200);
    
    while (CAN_OK != CAN_INTERFACE.begin(CAN_500KBPS)) {
        Serial.println("CAN Interface init failed ... try again");
        delay(100);
    }
    Serial.println("CAN Interface init success!");
    delay(500);
    
    epos4_1.init();

    uint32_t posl[1] = {50000};
    epos4_1.moveToPosition(posl, 1);
    delay(5000);

    epos4_1.initCSP();
}

void csp_test()
{
    while(1)
    {
        startTime = millis();


        uint32_t amp = (uint32_t)(50000 * cos(dt));
        // uint32_t amp = 50000;

        uint32_t swapped = ((amp>>24)&0xff) | ((amp<<8)&0xff0000) | ((amp>>8)&0xff00) | ((amp<<24)&0xff000000);
        uint8_t *n2 = (uint8_t *) &swapped;

        unsigned char value[4] = {n2[3],n2[2],n2[1],n2[0]};

        epos4_1._can_interface->sendMsgBuf(0x201, 0, 4, value);

        // send sync
        epos4_1.sync();

        dt += 0.01;


        endTime = millis() - startTime;
        delay(10-endTime);
    }
}
void csv_test()
{
    while(1)
    {
        startTime = millis();


        uint32_t amp = (uint32_t)(-5000 * sin(dt));
        //uint32_t amp = 50000;

        uint32_t swapped = ((amp>>24)&0xff) | ((amp<<8)&0xff0000) | ((amp>>8)&0xff00) | ((amp<<24)&0xff000000);
        uint8_t *n2 = (uint8_t *) &swapped;

        unsigned char value[4] = {n2[3],n2[2],n2[1],n2[0]};

        epos4_1._can_interface->sendMsgBuf(0x301, 0, 4, value);

        // send sync
        epos4_1.sync();

        dt += 0.01;


        endTime = millis() - startTime;
        delay(10-endTime);
    }
}
void cst_test()
{
    while(1)
    {
        startTime = millis();


        uint32_t amp = (uint32_t)(-100 * sin(dt));
        //uint32_t amp = 50000;

        uint32_t swapped = ((amp>>24)&0xff) | ((amp<<8)&0xff0000) | ((amp>>8)&0xff00) | ((amp<<24)&0xff000000);
        uint8_t *n2 = (uint8_t *) &swapped;

        unsigned char value[2] = {n2[3],n2[2]};

        epos4_1._can_interface->sendMsgBuf(0x401, 0, 2, value);

        // send sync
        epos4_1.sync();

        dt += 0.01;


        endTime = millis() - startTime;
        delay(10-endTime);
    }
}

void loop()
{

    csp_test();

}
