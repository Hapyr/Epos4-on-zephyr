#include "epos4.h"

#define MAX_DATA_SIZE 64

unsigned char tmp_data[MAX_DATA_SIZE] = {0};
int32_t tmpper = 0;
uint8_t tmp_len;
uint32_t tmp_id;
uint8_t tmp_type;

int32_t velo_inti = 0x00;

void tx_irq_callback(uint32_t error_flags, void *arg)
{
        char *sender = (char *)arg;

        if (error_flags) {
                printk("Sendig failed [%d]\nSender: %s\n", error_flags, sender);
        }
}

epos4::epos4(uint32_t canid_epos, uint32_t canid_self, const struct device *CAN_INTERFACE, struct k_msgq *my_can_msgq1, struct k_msgq *my_can_msgq2, struct zcan_frame *rx_frame):
    _canid_epos(canid_epos),
    _canid_self(canid_self),
    _softwareVersion(0),
    _hardwareVersion(0),
    _can_interface(CAN_INTERFACE),
    _my_can_msgq1(my_can_msgq1),
    _my_can_msgq2(my_can_msgq2),
    _rx_frame(rx_frame){}

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

void epos4::initSI()
{
    epos4::writeControlword(0x00, epos4::dcc_shutdown);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    char velocity_si[8] = {0x22, 0xA9, 0x60, 0x00, 0x00, 0x47, 0xB4, 0xFD}; // FD is maximum
    
    can_write(epos4::_can_interface,(uint8_t*)velocity_si,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    epos4::writeControlword(0x00, epos4::dcc_switchon_and_enable);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
}

void epos4::initPPM()
{
    printk("Set Operationmode");
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x01, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

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
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x08, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
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
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x09, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data01,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
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
    unsigned char data0[8] = {0x22, 0x60, 0x60, 0x00, 0x0A, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data0,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    // Interpolation time period value
    // Firmware spec. 6.2.121.1 Interpolation time period value ( 0x0a ==> 10ms )
    unsigned char data01[8] = {0x22, 0xC2, 0x60, 0x01, 0x0A, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data01,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

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


void epos4::sync()
{   
    struct zcan_frame frame = {
        .id = 0x80,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 1,
        .data = {0x00}
    };
    
    if( 0 != can_send(epos4::_can_interface, &frame, K_FOREVER, tx_irq_callback, NULL) )
    {
        printk("\n --- Error while sending sync frame --- \n");
    }

	k_msgq_get(epos4::_my_can_msgq2, epos4::_rx_frame, K_FOREVER);
    char *dat = (char*)(epos4::_rx_frame->data);
    epos4::reciveCANdataSPO( dat );
	// epos4::printFrame( epos4::_rx_frame );
    k_msgq_cleanup(epos4::_my_can_msgq2);

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
void epos4::reciveCANdataSPO(char *frame)
{
    int32_t *pos = (int32_t *)frame;
    int16_t *tor = (int16_t *)(frame+4);

    epos4::position = *pos;
    epos4::torque = *tor;
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
    can_write(epos4::_can_interface,(uint8_t*)data,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
}
void epos4::readStatusword()
{
    unsigned char data[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};    

    printk("\n --> ask for statusword\n");
	
    can_write(epos4::_can_interface,(uint8_t*)data,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

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

    unsigned char data[8] = {0x22, 0x7A, 0x60, 0x00, position[3], position[2], position[1], position[0]};
    //unsigned char data[8] = {0x22, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

    k_sleep(K_MSEC(10));

    unsigned char data1[8] = {0x22, 0x40, 0x60, 0x00, 0x1F, 0x00, 0x00, 0x00};
    can_write(epos4::_can_interface,(uint8_t*)data1,8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);

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

        moveToPosition(n2);
        k_sleep(K_MSEC(1));

        // printk("Toggle new poisition ...");
        epos4::writeControlword(0x00, 0x0F);
        k_sleep(K_MSEC(1));
    }
}

void epos4::configPDO() {

    // State Maschinbe of MNT Service to Pre Operational to checnge PDOs
    can_write(epos4::_can_interface,(uint8_t*)(epos4::enter_preoperational),2,0x00,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));

    // Config of TxPDO 1
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_transmissiontype_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_inhibit_time_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_null_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_position_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_velocity_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_TxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of TxPDO 2
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_transmissiontype_TxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_inhibit_time_TxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_null_TxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_torque_TxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_TxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    



    // Config of RxPDO 1
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_transmissiontype_RxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO3),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO1),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of RxPDO 2
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_transmissiontype_RxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO2),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    

    // Config of RxPDO 3
    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_transmissiontype_RxPDO3),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_null_RxPDO3),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_first_mapped_obj_position_RxPDO3),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );

    can_write(epos4::_can_interface,(uint8_t*)(epos4::pdo_number_of_mapped_objects_RxPDO3),8,epos4::_canid_self,CAN_DATAFRAME,K_FOREVER);
    k_sleep(K_MSEC(10));
	k_msgq_get(epos4::_my_can_msgq1, epos4::_rx_frame, K_FOREVER);
	epos4::printFrame( epos4::_rx_frame );
    


    // Set Statemaschine of NMT service back to Operational
    can_write(epos4::_can_interface,(uint8_t*)(epos4::start_remotenode),2,0x00,CAN_DATAFRAME,K_FOREVER);
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
    printk(*id, HEX);
    printk(" | ");

    char buffer[2] = {0};

    for( uint8_t i = 0; i < *len; i += 1 )
    {
        // printk(data[i],HEX);
        sprintf(buffer, "%02x", data[i]); 
        printk(buffer);
        printk(" ");
    }
    printk("");
}
*/
