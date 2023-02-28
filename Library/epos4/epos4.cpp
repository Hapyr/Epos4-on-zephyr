#include "epos4.h"

#define MAX_DATA_SIZE 64

unsigned char tmp_data[MAX_DATA_SIZE] = {0};
int32_t tmpper = 0;
uint8_t tmp_len;
uint32_t tmp_id;
uint8_t tmp_type;

int32_t velo_inti = 0x00;

void tx_irq_callback(const struct device *CAN_INTERFACE, int error_flags, void *arg)
{ 
    if (error_flags) {
        // printk("Sendig failed [%d]\nSender: \n", error_flags);
    }
}

void tx_irq_callback_mutex(const struct device *CAN_INTERFACE, int error_flags, void *arg)
{
    struct k_mutex *sender = (struct k_mutex *)arg;
    k_mutex_unlock(sender);
	// 
    // if (error_flags) {
    //     printk("Sendig failed [%d]\nSender: %s\n", error_flags, sender);
    // }
}

epos4::epos4(uint32_t canid_epos, uint32_t canid_self, const struct device *CAN_INTERFACE, struct k_msgq *my_can_msgq1, struct k_msgq *my_can_msgq2, struct zcan_frame *rx_frame, struct k_mutex *blocker_can):
    _canid_epos(canid_epos),
    _canid_self(canid_self),
    _softwareVersion(0),
    _hardwareVersion(0),
    _can_interface(CAN_INTERFACE),
    _my_can_msgq1(my_can_msgq1),
    _my_can_msgq2(my_can_msgq2),
    _blocker_can(blocker_can),
    _rx_frame(rx_frame){}

/**
 * @brief Inizialization of the controller. 
 * 
 * @details Depending on the status of the Epos4, it must be activated with a different "DeviceControlCommand". 
 * The exact flowchart for this can be taken from the Epos4 documentation.
 */
void epos4::init()
{
    // get state of drive 
    epos4::readStatusword();
    
    switch( epos4::_state_of_drive )
    {
        case epos4::Switch_on_disabled:
            epos4::writeControlword(0x00, epos4::dcc_shutdown);
		    k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
		    k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            break;

        case epos4::Ready_to_switch_on:
            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            break;

        case epos4::Switched_on:
            epos4::writeControlword(0x00, epos4::dcc_enableoperation);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            break;

        case epos4::Quick_stop_active:
            epos4::writeControlword(0x00, epos4::dcc_enableoperation);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            break;

        case epos4::Fault:
            epos4::writeControlword(0x00, epos4::dcc_faultreset);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            epos4::writeControlword(0x00, epos4::dcc_shutdown);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );

            epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
            k_sleep(K_MSEC(10));
	        k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	        epos4::printFrame( epos4::_rx_frame );
            break;

        case epos4::Operation_enabled:
            printk("Drive function is enabled and power is applied to the motor. ");
            break; 

        default: 
            printk("State of drive not found in enum ... may some error occured");
    }
    k_msgq_cleanup(epos4::_my_can_msgq1);

    printk("\n --> Start SI config ... \n");
    epos4::initSI();
    k_msgq_cleanup(epos4::_my_can_msgq1);
    printk("     done!\n");

    printk("\n --> Start TPDO config ... \n");
    epos4::configPDO();
    k_msgq_cleanup(epos4::_my_can_msgq1);
    printk("     done!\n");

    printk(" --- EPOS enabled --- ");
}

/**
 * @brief Abstration of the CAN interface. 
 * 
 * @details In most cases where data is sent via the CAn interface, this function is used. 
 * This way it is easier to adapt the CAN interface in case of changes in the Zephyr source code.
 */
void epos4::sendFrame(uint8_t *data_pointer, uint8_t size, uint32_t canid)
{
    zcan_frame frame;
        frame.id = canid;
        frame.rtr = CAN_DATAFRAME;
        frame.id_type = CAN_STANDARD_IDENTIFIER;
        frame.dlc = size;
        memcpy(frame.data, data_pointer, size);

    can_send(epos4::_can_interface, &frame, K_FOREVER, tx_irq_callback, NULL);
}

/**
 * @brief Configure the SI units on the Epos4. 
 * 
 * @details For different accuracies and value ranges, the power of ten of the measured values can be specified on the Epos4. 
 * For details look in  the Epos4 Firmware specs 6.2.115 SI unit position and 6.2.116 SI unit velocity
 */
void epos4::initSI()
{

    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // 0xFAB44700
    // 0x00B44700
    uint8_t velocity_si[8] = {0x22, 0xA9, 0x60, 0x00, 0x00, 0x47, 0xB4, 0xFD}; // FD is maximum 

    epos4::sendFrame(velocity_si, 8, epos4::_canid_self);

    //// max.: 0x2625A0 --> 2.500.000
    uint8_t digital_encode_1_resolution[8] = {0x22, 0x10, 0x30, 0x01, 0xA0, 0x25, 0x26, 0x00}; 
    epos4::sendFrame(digital_encode_1_resolution, 8, epos4::_canid_self);
    
    k_sleep(K_MSEC(100));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    k_sleep(K_MSEC(100));


    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    
}

/**
 * @brief Inizialize the Profile Position mode
 * 
 * @details Before using an operating mode on the Epos, it must be activated. The following functions serve this purpose.
 * The different operating modes and how they work can be found in the Epos4 documentation.
 * @{
 */
void epos4::initPPM()
{
    printk("Set Operationmode");
    uint8_t data0[8] = {0x22, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};
    epos4::sendFrame(data0, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));

	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    // Set all Profile Position Mode Parametersa here ... 


    // --------------------------------------------------------

    printk("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10));

	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
}

void epos4::initCSP()
{
    printk("Set Operationmode to CSP");
    uint8_t data0[8] = {0x22, 0x60, 0x60, 0x00, 0x08, 0x00, 0x00, 0x00};
    epos4::sendFrame(data0, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    uint8_t data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    epos4::sendFrame(data01, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );


    printk("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

}

void epos4::initCSV()
{
    printk("Set Operationmode to CSV");
    uint8_t data0[8] = {0x22, 0x60, 0x60, 0x00, 0x09, 0x00, 0x00, 0x00};
    epos4::sendFrame(data0, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    uint8_t data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    epos4::sendFrame(data01, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data01,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );


    printk("Switch on and enable ...");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10)); 
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
}

void epos4::initCST()
{
    printk("\n --> Set Operationmode to CST\n");
    uint8_t data0[8] = {0x22, 0x60, 0x60, 0x00, 0x0A, 0x00, 0x00, 0x00};
    epos4::sendFrame(data0, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    // Firmware spec. 6.2.121.1 Interpolation time period value ( 0x0a ==> 10ms )
    // Firmware spec. 6.2.121.1 Interpolation time period value ( 0x01 ==> 1ms )
    uint8_t data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x01, 0x00, 0x00, 0x00};
    epos4::sendFrame(data01, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data01,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );


    printk(" --> Switch on and enable \n");
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10)); 
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
}
/** @} */

/**
 * @brief Syncronization function for the cyclic modes. 
 * 
 * @details In the cyclic modes, the Epos4 expects new values at regular intervals. 
 * This function only sends a sync frame so that the Epos4 knows that we are still synced and can send the new measured values.
 * Due to a bug in the Zephyr CAN interface the send function for the CAN interface was protected with a mutex. 
 * The mutex is removed as soon as 'tx_irq_callback_mutex' is called. This ensures that only when zephyr has left the send routine for CAN, a new CAN frame is tried to be sent. 
 */
void epos4::sync()
{   
    struct zcan_frame frame = {
        .id = 0x80,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 1,
        .data = {0x00}
    };

    /*
    get unlocked after 'tx_irq_callback_mutex'
    */
    k_mutex_lock(epos4::_blocker_can, K_FOREVER);
    
    int can_send_return = can_send(epos4::_can_interface, &frame, K_FOREVER, tx_irq_callback_mutex, epos4::_blocker_can);
	
    // Some Printout if error
    if(can_send_return != 0){
		//uint32_t asdf = (uint32_t)(can_send_return);
		//uint8_t *asdf_t;
		//asdf_t = (uint8_t *)(&asdf);
		//
		//uint8_t err_msg[5] = {asdf_t[0],asdf_t[1],asdf_t[2],asdf_t[3],'\n'};
		//uart_tx(epos4_1_pointer->_can_interface, err_msg, 5, 0);
	}
	// k_msgq_get(epos4::_my_can_msgq2, epos4::_rx_frame, K_USEC(100));
    // 
    // epos4::reciveCANdataSPO( (char*)(epos4::_rx_frame->data) );
	// // epos4::printFrame( epos4::_rx_frame );
    // k_msgq_cleanup( epos4::_my_can_msgq2 );

}

void epos4::reciveCANdata(char *frame)
{
    if( frame[1] == 0x41 || frame[2] == 0x60 )
	{
		uint16_t statusword = ( frame[5] << 8 ) | ( frame[4] );
    	epos4::_target_reached = statusword & 0b0000010000000000;
    	epos4::_state_of_drive = statusword & 0b01101111;
	}
}

/**
 * @brief Save the measured values of the Epic in Cyclic mode
 * 
 * @details In the cyclic modes, new measured values are sent at regular time intervals. 
 * Depending on the 'measure_mode', position and speed or position and torque are sent and stored. 
 */
void epos4::reciveCANdataSPO(char *frame)
{
    int32_t *pos = (int32_t *)frame;
    epos4::position = *pos;

    if( epos4::measure_mode == 0 ){ 
        int32_t *vel = (int32_t *)(frame+4);
        epos4::velocity = *vel;
    }else{
        int16_t *tor = (int16_t *)(frame+4);                           
        epos4::torque = *tor;
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
    epos4::sendFrame(data, 8, epos4::_canid_self);
}
void epos4::readStatusword()
{
    unsigned char data[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};    

    printk("\n --> ask for statusword\n");
    epos4::sendFrame(data, 8, epos4::_canid_self);
	
    printk("     Sended\n");

	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);

    char *dat = (char*)(epos4::_rx_frame->data);

	epos4::reciveCANdata(dat);
	epos4::printFrame( epos4::_rx_frame );

    while(epos4::_state_of_drive == 0x00)
    {
        printk("     Wait for Controllword...\n");
    }
    printk("     done!\n");

}

void epos4::printFrame(struct zcan_frame *frame)
{
	char *dat = (char*)(frame->data);
	printk("MSG: ");
	
	for(int i = 0; i < 8; i+=1)
	{
		printk("%x",(unsigned char)(dat[i]));
	}
	
	printk("\n");
}

void epos4::moveToPosition(uint8_t *position)
{
    // position is an 32 bit integer. Wen give the uint8 here because we can interate on the bytes from the uint32
    printk("Starte move to position ... ");

    uint8_t data[8] = {0x22, 0x7A, 0x60, 0x00, position[3], position[2], position[1], position[0]};
    //unsigned char data[8] = {0x22, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
    epos4::sendFrame(data, 8, epos4::_canid_self);
    // can_write(epos4::_can_interface,(uint8_t*)data,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));

    uint8_t data1[8] = {0x22, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00};
    epos4::sendFrame(data1, 8, epos4::_canid_self);

    // can_write(epos4::_can_interface,(uint8_t*)data1,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));
   

    printk("done!");
    
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

        epos4::moveToPosition(n2);
        k_sleep(K_MSEC(1));

        // printk("Toggle new poisition ...");
        epos4::writeControlword(0x00, 0x0F);
        k_sleep(K_MSEC(2000));
    }
}

/**
 * @brief Configuration of the PDOs of the Epos4
 * 
 * @details The PDO objects of the Epos4 must be configured in a certain way. 
 * Several frames must be sent per PDO for configuration.
 *  Which ones exactly and in which sequence can be looked up in the Epos4 documentation. 
 * In the epos4.h the PDO frames can be adapted to generate other behavior. 
 */
void epos4::configPDO() {

    // State Maschinbe of MNT Service to Pre Operational to checnge PDOs

    epos4::sendFrame((uint8_t*)(epos4::enter_preoperational),2, 0x00);
    k_sleep(K_MSEC(10));

    // Config of TxPDO 1
    epos4::sendFrame((uint8_t*)(epos4::pdo_transmissiontype_TxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_inhibit_time_TxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_null_TxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_position_TxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    if( epos4::measure_mode == 0 ){
        epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_velocity_TxPDO1), 8, epos4::_canid_self);
    }else{
        epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_torque_TxPDO1), 8, epos4::_canid_self);
    }
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_TxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of TxPDO 2
    epos4::sendFrame((uint8_t*)(epos4::pdo_transmissiontype_TxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_inhibit_time_TxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_null_TxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_torque_TxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_TxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    



    // Config of RxPDO 1
    epos4::sendFrame((uint8_t*)(epos4::pdo_transmissiontype_RxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO3), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO1), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of RxPDO 2
    epos4::sendFrame((uint8_t*)(epos4::pdo_transmissiontype_RxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO2), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of RxPDO 3
    epos4::sendFrame((uint8_t*)(epos4::pdo_transmissiontype_RxPDO3), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO3), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO3), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::sendFrame((uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO3), 8, epos4::_canid_self);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Set Statemaschine of NMT service back to Operational
    epos4::sendFrame((uint8_t*)(epos4::start_remotenode), 2, 0x00);
    k_sleep(K_MSEC(100));

    printk("TPDO Configured!");
}

/*
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
    char getSoftwareVersion[8] = {0x40, 0x18, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00};

    epos4::_can_interface->sendMsgBuf(epos4::_canid_self, 0, 8, getSoftwareVersion);

    unsigned char data[MAX_DATA_SIZE] = {0};
    uint8_t len;
    uint32_t id;
    uint8_t type;

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
*/
