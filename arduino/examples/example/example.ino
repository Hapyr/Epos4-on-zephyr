#include "Arduino.h"

#include "epos4.h"

#define MAX_DATA_SIZE 64
#define SPI_CS_PIN 9
#define CAN_INT_PIN 2
#define DT 0.01
#define KP 5.2
#define KI 7.0

mcp2515_can CAN_INTERFACE(SPI_CS_PIN);
epos4 epos4_1(0x1,0x601, &CAN_INTERFACE);

volatile boolean gchecker = false;
volatile boolean toor = false;
volatile int dt = 0;

// Time | input | output
volatile unsigned long daten_time[400];
volatile uint32_t daten_input[400];
volatile int16_t daten_input_print[400];
volatile uint32_t daten_output[400];

// need to think about this later
unsigned long startTime;
unsigned long endTime;
unsigned char value[2] = {0};

void setup()
{
    int ccount = 0;
    for(float i = 0.0; i < 4.0; i+=0.01)
    {
        daten_time[ccount] = 0.0;
        daten_output[ccount] = 0.0;
        
        uint32_t inValue;

        if( i >= 1.0 && i <= 1.5) inValue = (uint32_t)(300);
        //else if( i >=1.0 && i <= 1.5 ) inValue = (uint32_t)(-100);
        //else if( i >=1.9 && i <= 2.4 ) inValue = (uint32_t)(150);
        //else if( i >=2.4 && i <= 3.0 ) inValue = (uint32_t)(-100);
        else inValue = (uint32_t)(0);

        daten_input[ccount] = ((inValue>>24)&0xff) | ((inValue<<8)&0xff0000) | ((inValue>>8)&0xff00) | ((inValue<<24)&0xff000000);

        ccount += 1;
    }

    // Interupt stuff
    cli();

    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 2499;// = (16*10^6) / (1*64) - 1 (must be <65536)
    
    TCCR1B |= (1 << WGM12);//for timer1
    TCCR1B |= (1 << CS10); 
    TCCR1B |= (1 << CS11); 

    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    sei();
    
    Serial.begin(115200);
    
    while (CAN_OK != CAN_INTERFACE.begin(CAN_500KBPS)) {
        Serial.println("CAN Interface init failed ... try again");
        delay(100);
    }
    Serial.println("CAN Interface init success!");
    delay(500);
    
    epos4_1.init();

    uint32_t posl[1] = {0};
    epos4_1.moveToPosition(posl, 1);
    delay(5000);

    epos4_1.initCST();
    gchecker = true;
    toor = true;
}

void csp_test()
{
    while(1)
    {
        startTime = micros();


        uint32_t amp = (uint32_t)(50000 * cos(dt));
        // uint32_t amp = 50000;

        uint32_t swapped = ((amp>>24)&0xff) | ((amp<<8)&0xff0000) | ((amp>>8)&0xff00) | ((amp<<24)&0xff000000);
        uint8_t *n2 = (uint8_t *) &swapped;

        unsigned char value[4] = {n2[3],n2[2],n2[1],n2[0]};

        epos4_1._can_interface->sendMsgBuf(0x201, 0, 4, value);

        // send sync
        Serial.print("Data===");
        Serial.print(String(dt));
        epos4_1.sync();

        dt += 0.01;


        endTime = micros() - startTime;
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
    if(dt<400 && gchecker == true)
    {
        uint8_t *n2 = (uint8_t *)(daten_input + dt);

        value[0] = n2[3];
        value[1] = n2[2];

        epos4_1.sync();

        daten_time[dt] = micros();
        daten_input_print[dt] = (int16_t)epos4_1.torque;
        daten_output[dt] = (int32_t)epos4_1.position;

        dt += 1;
    }else if(dt >= 400 && gchecker == true){

        for(int i=0; i < 400; i+=1)
        {
            Serial.print(String(i) + ",");
            Serial.print(String(daten_time[i]) + ",");
            Serial.print(String((int16_t)(daten_input_print[i])) + ",");
            Serial.println(String((int32_t)(daten_output[i])));
        }

        gchecker = false;
    }
}

//timer1 interrupt 100Hz
ISR(TIMER1_COMPA_vect)
{
    if(toor)
    {
        toor = false;
        epos4_1._can_interface->sendMsgBuf(0x401, 0, 2, value);
        cst_test();
        toor = true;
    }
}

void loop()
{
}
