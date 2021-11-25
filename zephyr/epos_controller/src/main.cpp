#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

#include "epos4/epos4.h"

#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2
#define STATE_POLL_THREAD_STACK_SIZE 512
#define STATE_POLL_THREAD_PRIORITY 2
#define LED_MSG_ID 0x10
#define COUNTER_MSG_ID 0x12345
#define SET_LED 1
#define RESET_LED 0
#define SLEEP_TIME K_MSEC(250)

K_THREAD_STACK_DEFINE(rx_thread_stack, RX_THREAD_STACK_SIZE);

#define CAN_DEVICE_LABEL DT_PROP(DT_ALIAS(can_dev), label)

const struct device* can_dev;
// const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_can_primary));

epos4* epos4_1_pointer = NULL;


struct k_thread rx_thread_data;
struct k_thread poll_state_thread_data;
struct zcan_work rx_work;
struct k_work state_change_work;
enum can_state current_state;
struct can_bus_err_cnt current_err_cnt;

void rx_callback_function(struct zcan_frame *frame, void *arg)
{
	char *dat = (char*)(frame->data);

	if( dat[1] == 0x41 || dat[2] == 0x60 )
	{
		uint16_t statusword = ( dat[5] << 8 ) | ( dat[4] );
    	epos4_1_pointer->_target_reached = statusword & 0b0000010000000000;
    	epos4_1_pointer->_state_of_drive = statusword & 0b01101111;
	}

	printk("MSG: ");
	
	for(int i = 0; i < 8; i+=1)
	{
		printk("%x",(unsigned char)(dat[i]));
	}
	
	printk("\n");
}



void main(void)
{
	epos4 epos4_1(0x581,0x601, can_dev);
	epos4_1_pointer = &epos4_1;
	epos4_1_pointer->_can_interface = device_get_binding("CAN_1");

	k_tid_t rx_tid;

    if (!device_is_ready(epos4_1_pointer->_can_interface)) {
		printk("CAN: Device %s not ready.\n", epos4_1_pointer->_can_interface->name);
		return;
	}else{
        printk("CAN device %s is ready...\n", epos4_1_pointer->_can_interface->name);
    }
	can_set_mode(epos4_1_pointer->_can_interface, CAN_NORMAL_MODE);

	const struct zcan_filter my_filter = {
        .id = 0x581,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .id_mask = CAN_STD_ID_MASK,
        .rtr_mask = 1
	};
	int filter_id;

	filter_id = can_attach_isr(epos4_1_pointer->_can_interface, rx_callback_function, NULL, &my_filter);
	if (filter_id < 0) {
	  	printk("Unable to attach isr [%d]", filter_id);
	}

	printk("start init...");
    epos4_1_pointer->init();
	printk(" done!\nStart init CST-Mode: ");
    epos4_1_pointer->initCST();

	uint32_t posList[2] = {0,50000};
	while(1)
	{
		k_sleep(K_MSEC(7000));
		epos4_1_pointer->moveToPosition(posList,2);
		// printk("Test ... \n");
	}
}