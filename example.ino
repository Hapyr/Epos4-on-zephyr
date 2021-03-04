#include "Arduino.h"

#include "mcp2515_can.h"
#include "mcp2515_can_dfs.h"



#include <epos4.h>

#define MAX_DATA_SIZE 64
#define SPI_CS_PIN 9
#define CAN_INT_PIN 2
#define DT 0.01
#define KP 5.2
#define KI 7.0

mcp2515_can CAN_INTERFACE(SPI_CS_PIN);
epos4 epos4_1(0x581,0x601, &CAN_INTERFACE);

float dt = 0.0;
float integralSum = 0.0;
//float Kp = 0.018377329;
float Kp = 0.088377329;
float Ki = 0.019424878;
//float Ki = 0.06424878;
unsigned long startTime;
unsigned long endTime;

int32_t lastPos = 0x00;
int32_t lastPosnew = 0x00;

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

    uint32_t posl[1] = {200};
    epos4_1.moveToPosition(posl, 1);
    delay(5000);

    epos4_1.initCST();
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

        uint32_t amp = 0;
        float resser = 0.0;

        // Controlling position with PI controller 
        integralSum += ( DT * ((float)lastPos) + ( ( ((float)lastPosnew - (float)lastPos)  ) * DT/2.0 ) );
        if(integralSum > 20000000.0) integralSum = 20000000.0;
        else if(integralSum < -20000000.0) integralSum = -20000000.0;
        resser = Kp * lastPosnew + Ki * integralSum;
        amp = (uint32_t)resser;
        
        /*
        if( dt >= 1.0 && dt <= 2.0) amp = (uint32_t)(200);
        else if( dt >=3.0 && dt <= 4.0 ) amp = (uint32_t)(-100);
        else if( dt >=5.0 && dt <= 6.0 ) amp = (uint32_t)(-300);
        else if( dt >=7.0 && dt <= 8.0 ) amp = (uint32_t)(300);
        else amp = (uint32_t)(0);
        */
        // uint32_t amp = (uint32_t)(-100 * sin(dt));
        // uint32_t amp = 50000;

        // Serial.print(String(dt));
        int32_t *ptr = (int32_t *)(&amp);
            
        char buffer[10] = {0};
        sprintf(buffer, "Drehmoment: %li" , *ptr);
        Serial.print(buffer);

        uint32_t swapped = ((amp>>24)&0xff) | ((amp<<8)&0xff0000) | ((amp>>8)&0xff00) | ((amp<<24)&0xff000000);
        uint8_t *n2 = (uint8_t *) &swapped;

        unsigned char value[2] = {n2[3],n2[2]};

        epos4_1._can_interface->sendMsgBuf(0x401, 0, 2, value);

        // send sync
        epos4_1.sync(&lastPos,&lastPosnew);

        dt += DT;


        endTime = millis() - startTime;
        delay(10-endTime);
    }
}

void loop()
{

    cst_test();

}
