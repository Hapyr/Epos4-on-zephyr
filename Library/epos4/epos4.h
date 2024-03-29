#ifndef epos4_h
#define epos4_h

#include <zephyr.h>
#include <kernel.h>
#include <drivers/uart.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

class epos4
{
public:
    epos4(uint32_t canid_epos, uint32_t canid_self, const struct device *CAN_INTERFACE, struct k_msgq *my_can_msgq1, struct k_msgq *my_can_msgq2, struct zcan_frame *rx_frame, struct k_mutex *blocker_can);
    
    void init();
    void initSI();
    void initPPM();
    void initCSP();
    void initCSV();
    void initCST();
    void sync();
    void printFrame(struct zcan_frame *frame);
    void reciveCANdata(char *frame);
    void reciveCANdataSPO(char *frame);

    void moveToPosition(uint32_t *posList, uint8_t len);
    void getVersions(uint16_t *hardware_version, uint16_t *application_number, uint16_t *software_version, uint16_t *application_version);

    void writeControlword(unsigned char highByte, unsigned char lowByte);
    void readStatusword();

    void checkReceivedData(unsigned char *data, uint8_t *len, uint32_t *id, uint8_t *type,bool *check=NULL);
    void printCANframe(unsigned char *data, uint8_t *len, uint32_t *id, uint8_t *type);
    void sendFrame(uint8_t *data_pointer, uint8_t size, uint32_t canid = 0x00);
    const struct device *_can_interface;
    struct k_mutex *_blocker_can;

    int32_t position = 0x00; 
    int32_t velocity = 0x00; 
    int16_t torque = 0x00; 

    /**
    * @addtogroup DeviceControlCommands
    * @brief Device Control Commands
    * @details With the measuring mode we can decide which measured values are sent cyclically by the Epos controller. When zero, it sends the position and speed. When unequal zero, it sends position and torque. 
    * @{
    */
    int16_t measure_mode = 0;
    /** @} */
    
    bool data_mutex = true;

    int _softwareVersion;
    int _hardwareVersion;

    volatile uint8_t _state_of_drive = 0;
    bool _target_reached = false;

    // Statemaschine enum
    enum
    {
        Not_ready_to_switch_on  = 0b00000000,
        Switch_on_disabled      = 0b01000000,
        Ready_to_switch_on      = 0b00100001,
        Switched_on             = 0b00100011,
        Operation_enabled       = 0b00100111,
        Quick_stop_active       = 0b00000111,
        Fault_reaction_active   = 0b00001111,
        Fault                   = 0b00001000
    };
    
    /**
    * @addtogroup DeviceControlCommands
    * @brief Device Control Commands
    * @details For more details look in the EPOS4 Firmware Specification - 2.2 Device Control
    * @{
    */
    enum 
    {
        dcc_shutdown = 0x06,            /**< will end in axis state >ready to switch on< */
        dcc_switchon = 0x07,            /**< will end in axis state >switched on< */
        dcc_switchon_and_enable = 0x0F, /**< will end in axis state >switch on< then auto transition to >operation enabled< */
        dcc_disablevoltage = 0x00,      /**< will end in axis state >switch on disabled< */
        dcc_quickstop = 0x02,           /**< will end in axis state >quick stop active< */
        dcc_disableoperation = 0x07,    /**< will end in axis state >switched on< */
        dcc_enableoperation = 0x0F,     /**< will end in axis state >operation enabled< */
        dcc_faultreset = 0x80           /**< will end in axis state >Fault< and/or >switch on disabled< */
    };
    /** @} */

    /**
    * @addtogroup StatemaschineVariables
    * @brief Statemaschine for NMT
    * @details For more details look in the EPOS4 Communication Guide - 3.3.3.5 NMT Services
    * @{
    */
    unsigned char enter_preoperational[2] = {0x80, 0x00}; /**< All CANopen nodes will enter NMT state >Pre-operational< */
    unsigned char reset_communication[2] = {0x82, 0x00};  /**< All CANopen nodes will first enter NMT state >Initialization< and then switch automatically >Pre-operational< */
    unsigned char reset_node[2] = {0x81, 0x00};           /**< All CANopen nodes will first enter NMT state >Initialization< and then switch automatically >Pre-operational< */
    unsigned char start_remotenode[2] = {0x01, 0x00};     /**< All CANopen nodes will enter NMT state >Operational< */
    unsigned char stop_remotenode[2] = {0x02, 0x00};      /**< All CANopen nodes will enter NMT state >Stopped< */
    /** @} */


    /**
    * @addtogroup TxPDO config
    * @brief set config for transmition pdos
    * @details For more details look in the EPOS4 Communication Firmaware Specification - 6.2.21 Transmit PDO 1 parameter
    * 
    * @{
    */
    // TxPDO 1
    unsigned char pdo_transmissiontype_TxPDO1[8]                = {0x22, 0x00, 0x18, 0x02, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_inhibit_time_TxPDO1[8]                    = {0x22, 0x00, 0x18, 0x03, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_TxPDO1[8]        = {0x22, 0x00, 0x1A, 0x00, 0x02, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_null_TxPDO1[8]   = {0x22, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};

    unsigned char pdo_first_mapped_obj_position_TxPDO1[8]       = {0x23, 0x00, 0x1A, 0x01, 0x20, 0x00, 0x64, 0x60};     // 104 Grad = 120000 inc
    //unsigned char pdo_first_mapped_obj_velocity_TxPDO1[8]       = {0x23, 0x00, 0x1A, 0x02, 0x20, 0x00, 0x6C, 0x60};   // velocity [mrpm] | 6.2.98 Velocity actual value
    unsigned char pdo_first_mapped_obj_velocity_TxPDO1[8]       = {0x23, 0x00, 0x1A, 0x02, 0x20, 0x01, 0xE5, 0x60};     // velocity [mrpm] | 6.2.124.1 Velocity actual value sensor 1
    unsigned char pdo_first_mapped_obj_torque_TxPDO1[8]         = {0x22, 0x00, 0x1A, 0x02, 0x10, 0x00, 0x77, 0x60};     // torque*10**(-6) [Nm] 
    
    // TxPDO 2
    unsigned char pdo_transmissiontype_TxPDO2[8]                = {0x22, 0x01, 0x18, 0x02, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_inhibit_time_TxPDO2[8]                    = {0x22, 0x01, 0x18, 0x03, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_TxPDO2[8]        = {0x22, 0x01, 0x1A, 0x00, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_null_TxPDO2[8]   = {0x22, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char pdo_first_mapped_obj_torque_TxPDO2[8]         = {0x22, 0x01, 0x1A, 0x01, 0x10, 0x00, 0x77, 0x60};
    /** @} */
    
    // RxPDO 1
    unsigned char pdo_transmissiontype_RxPDO1[8]                = {0x22, 0x00, 0x14, 0x02, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_RxPDO1[8]        = {0x22, 0x00, 0x16, 0x00, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_null_RxPDO1[8]   = {0x22, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char pdo_first_mapped_obj_position_RxPDO1[8]       = {0x23, 0x00, 0x16, 0x01, 0x20, 0x00, 0x7A, 0x60};
    
    // RxPDO 2
    unsigned char pdo_transmissiontype_RxPDO2[8]                = {0x22, 0x01, 0x14, 0x02, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_RxPDO2[8]        = {0x22, 0x01, 0x16, 0x00, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_null_RxPDO2[8]   = {0x22, 0x01, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char pdo_first_mapped_obj_position_RxPDO2[8]       = {0x23, 0x01, 0x16, 0x01, 0x20, 0x00, 0xFF, 0x60};

    // RxPDO 3
    unsigned char pdo_transmissiontype_RxPDO3[8]                = {0x22, 0x02, 0x14, 0x02, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_RxPDO3[8]        = {0x22, 0x02, 0x16, 0x00, 0x01, 0x00, 0x00, 0x00};
    unsigned char pdo_number_of_mapped_objects_null_RxPDO3[8]   = {0x22, 0x02, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char pdo_first_mapped_obj_position_RxPDO3[8]       = {0x23, 0x02, 0x16, 0x01, 0x10, 0x00, 0x71, 0x60};

private:
    void getProductCode(uint16_t *hardware_version, uint16_t *application_number);
    void getRevisionNumber(uint16_t *software_version, uint16_t *application_version);
    void configPDO();
    void moveToPosition(uint8_t *position);

    uint32_t _canid_epos;
    uint32_t _canid_self;
    uint32_t _spi_cs_pin;
    uint32_t _can_int_pin;
    uint32_t _state;

    struct k_msgq *_my_can_msgq1;
    struct k_msgq *_my_can_msgq2;
    struct zcan_frame *_rx_frame;
    

};

#endif
