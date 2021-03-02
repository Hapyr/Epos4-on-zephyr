#include "Arduino.h"
#include <SPI.h>

#include "epos4.h"
#include "mcp2515_can.h"
#include "mcp2515_can_dfs.h"

#define MAX_DATA_SIZE 64


unsigned char tmp_data[MAX_DATA_SIZE] = {0};
int32_t tmpper = 0;
uint8_t tmp_len;
uint32_t tmp_id;
uint8_t tmp_type;

epos4::epos4(int canid_epos, int canid_self, mcp2515_can *CAN_INTERFACE)
{
    _canid_epos = canid_epos;
    _canid_self = canid_self;
    _softwareVersion = 0;
    _hardwareVersion = 0;
    _can_interface = CAN_INTERFACE;
}

void epos4::init()
{
    // get state of drive 
    epos4::readStatusword();
    
    switch( epos4::_state_of_drive )
    {
        case epos4::Switch_on_disabled:
            epos4::writeControlword(0x00, epos4::dcc_shutdown);
            delay(10);
            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
            delay(10);
            break;

        case epos4::Ready_to_switch_on:
            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
            delay(10);
            break;

        case epos4::Switched_on:
            epos4::writeControlword(0x00, epos4::dcc_enableoperation);
            delay(10);
            break;

        case epos4::Quick_stop_active:
            epos4::writeControlword(0x00, epos4::dcc_enableoperation);
            delay(10);
            break;

        case epos4::Fault:
            epos4::writeControlword(0x00, epos4::dcc_faultreset);
            delay(10);
            epos4::writeControlword(0x00, epos4::dcc_shutdown);
            delay(10);
            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
            delay(10);
            break;

        case epos4::Operation_enabled:
            Serial.println("Drive function is enabled and power is applied to the motor. ");
            break; 

        default: 
            Serial.println("State of drive not found in enum ... may some error occured");
    }
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    Serial.print("Start SI config ... ");
    epos4::initSI();
    Serial.println("done!");

    Serial.println("Start TPDO config ... ");
    epos4::configPDO();
    Serial.println("done!");

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    Serial.println("EPOS enabled");
}

void epos4::initSI()
{
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    delay(10);

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    char velocity_si[8] = {0x22, 0xA9, 0x60, 0x00, 0x00, 0x47, 0xB4, 0xFD}; // FD is maximum
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, velocity_si);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    delay(10);
}

void epos4::initPPM()
{
    Serial.println("Set Operationmode");
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data0);
    delay(10);

    // Set all Profile Position Mode Parametersa here ... 


    // --------------------------------------------------------

    Serial.println("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    delay(10);
}

void epos4::initCSP()
{
    Serial.println("Set Operationmode to CSP");
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x08, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data0);
    delay(10);

    // Interpolation time period value
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data01);
    delay(10);


    Serial.println("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    delay(10);
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    delay(10);

}

void epos4::initCSV()
{
    Serial.println("Set Operationmode to CSV");
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x09, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data0);
    delay(10);

    // Interpolation time period value
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data01);
    delay(10);


    Serial.println("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    delay(10);
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    delay(10); 
}

void epos4::initCST()
{
    Serial.println("Set Operationmode to CST");
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x0A, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data0);
    delay(10);

    // Interpolation time period value
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data01);
    delay(10);


    Serial.println("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    delay(10);
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    delay(10); 
}

void epos4::sync()
{
    epos4::_can_interface->sendMsgBuf(0x80, 0, 1, 0x00);

    bool checker = false;
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type, &checker);
    
    if( checker == true )
    {
        if(tmp_id == 0x181)
        {
            int32_t *ptr = (int32_t *)tmp_data;
            
            char buffer[10] = {0};
            sprintf(buffer, "%li", *ptr);
            Serial.print("Position: ");
            Serial.print(buffer);
            Serial.print(" ");


            int32_t *ptr1 = (int32_t *)(tmp_data + 4);

            char buffer1[10] = {0};
            sprintf(buffer1, "%li", ((*ptr1) * 10));
            Serial.print("Velocity: ");
            Serial.println(buffer1);
        }

    }
}

/**
 * @brief Send a controlword with SDO communication
 * 
 * @details The first byte is the command specifier. For controlword its fixed with 0x2B because we want expedited transfer with const size of 2 Byte
 *          For more details look in >EPOS4 Firmware Specification< chapter 6.2.85 - Controlword and >EPOS4 Application Notes< chapter 5.4 - SDO Communication
 * 
 * @param highByte
 * @param lowByte
 */
void epos4::writeControlword(unsigned char highByte, unsigned char lowByte)
{
    unsigned char data[8] = {0x2B, 0x40, 0x60, 0x00, lowByte, highByte, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data);
}
void epos4::readStatusword()
{
    unsigned char data[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data);
    
    tmp_data[1] = 0x00;
    while(tmp_data[1] != 0x41 || tmp_data[2] != 0x60){
        epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    }
    Serial.print("State of Drive: ");
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    uint16_t statusword = ( tmp_data[5] << 8 ) | ( tmp_data[4] );

    epos4::_target_reached = statusword & 0b0000010000000000;
    epos4::_state_of_drive = statusword & 0b01101111;
}

void epos4::moveToPosition(uint8_t *position)
{
    // position is an 32 bit integer. Wen give the uint8 here because we can interate on the bytes from the uint32
    Serial.print("Starte move to position ... ");

    unsigned char data[8] = {0x22, 0x7A, 0x60, 0x00, position[3], position[2], position[1], position[0]};
    //unsigned char data[8] = {0x22, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data); 
    delay(10);

    unsigned char data1[8] = {0x22, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00};
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, data1);  
    delay(10);
   

    Serial.println("done!");
    
}

void epos4::moveToPosition(uint32_t *posList, uint8_t len)
{
    epos4::initPPM();

    uint32_t reversedPos;
    uint32_t printPos;
    uint8_t *n2;
    n2 = (uint8_t *) &reversedPos;

    for( uint8_t i = 0; i < len; i+= 1 )
    {
        n2[3] = (posList[i]);
        n2[2] = (posList[i]) >> 8;
        n2[1] = (posList[i]) >> 16;
        n2[0] = (posList[i]) >> 24;

        moveToPosition(n2);
        delay(1);

        // Serial.println("Toggle new poisition ...");
        epos4::writeControlword(0x00, 0x0F);
        delay(1);
    }
}

void epos4::configPDO() {

    // State Maschinbe of MNT Service to Pre Operational to checnge PDOs
    epos4::_can_interface->sendMsgBuf(0x00, 0, 2, epos4::enter_preoperational);
    delay(10);

    // Config of TxPDO 1
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_transmissiontype_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_inhibit_time_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_null_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_position_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_velocity_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_TxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    

    // Config of TxPDO 2
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_transmissiontype_TxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_inhibit_time_TxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_null_TxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_torque_TxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_TxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    



    // Config of RxPDO 1
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_transmissiontype_RxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_null_RxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_position_RxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_RxPDO1);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    

    // Config of RxPDO 2
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_transmissiontype_RxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_null_RxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_position_RxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_RxPDO2);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    

    // Config of RxPDO 3
    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_transmissiontype_RxPDO3);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_null_RxPDO3);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_first_mapped_obj_position_RxPDO3);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, epos4::pdo_number_of_mapped_objects_RxPDO3);
    delay(10);
    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);


    // Set Statemaschine of NMT service back to Operational
    epos4::_can_interface->sendMsgBuf(0x00, 0, 2, epos4::start_remotenode);
    delay(100);

    epos4::checkReceivedData(tmp_data, &tmp_len, &tmp_id, &tmp_type);
    epos4::printCANframe(tmp_data, &tmp_len, &tmp_id, &tmp_type);


    Serial.println("TPDO Configured!");
}

void epos4::getVersions(uint16_t* hardware_version, uint16_t* application_number, uint16_t* software_version, uint16_t* application_version)
{
    epos4::getProductCode(hardware_version,application_number);
    epos4::getRevisionNumber(software_version,application_version);
}
void epos4::getProductCode(uint16_t* hardware_version, uint16_t* application_number)
{
    unsigned char getHardwareVersion[8] = {0x40, 0x18, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00};

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, getHardwareVersion);

    unsigned char data[MAX_DATA_SIZE] = {0};
    uint8_t len;
    uint32_t id;
    uint8_t type;

    epos4::checkReceivedData(data, &len, &id, &type);
    epos4::printCANframe(data, &len, &id, &type);

    if( id == epos4::_canid_epos )
    {
        if( data[0] == 0x43 && data[1] == 0x18 && data[2] == 0x10 && data[3] == 0x02 )
        {
            (*hardware_version)     = (data[7]<<8) | data[6];
            (*application_number)   = (data[5]<<8) | data[4];
            epos4::_hardwareVersion = (*hardware_version);
        }
    }
}
void epos4::getRevisionNumber(uint16_t* software_version, uint16_t* application_version)
{
    byte getSoftwareVersion[8] = {0x40, 0x18, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00};

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, getSoftwareVersion);

    unsigned char data[MAX_DATA_SIZE] = {0};
    uint8_t len;
    uint32_t id;
    uint8_t type;

    epos4::checkReceivedData(data, &len, &id, &type);
    epos4::printCANframe(data, &len, &id, &type);

    if( id == epos4::_canid_epos )
    {
        if( data[0] == 0x43 && data[1] == 0x18 && data[2] == 0x10 && data[3] == 0x03 )
        {
            (*software_version)     = (data[7]<<8) | data[6];
            (*application_version)   = (data[5]<<8) | data[4];
            epos4::_softwareVersion = (*software_version);
        }
    }
}

void epos4::checkReceivedData(unsigned char *data, uint8_t *len, uint32_t *id, uint8_t *type, bool *check)
{
    if (CAN_MSGAVAIL == epos4::_can_interface->checkReceive()) 
    {
        epos4::_can_interface->readMsgBuf(len, data);
        *id     = epos4::_can_interface->getCanId();
        *type   =   (epos4::_can_interface->isExtendedFrame() << 0) |
                    (epos4::_can_interface->isRemoteRequest() << 1);
        if(check) *check = true;

    }else{
        if(check) *check = false;
        *id = 0x0;
    }
}

void epos4::printCANframe(unsigned char *data, uint8_t *len, uint32_t *id, uint8_t *type)
{
    Serial.print(*id, HEX);
    Serial.print(" | ");

    char buffer[2] = {0};

    for( uint8_t i = 0; i < *len; i += 1 )
    {
        // Serial.print(data[i],HEX);
        sprintf(buffer, "%02x", data[i]); 
        Serial.print(buffer);
        Serial.print(" ");
    }
    Serial.println("");
}
