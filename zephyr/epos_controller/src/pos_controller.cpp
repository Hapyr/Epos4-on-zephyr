#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "Atomic.h"
#ifdef __cplusplus
}
#endif

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


CAN_DEFINE_MSGQ(my_can_msgq1, 100);
CAN_DEFINE_MSGQ(my_can_msgq2, 100);
struct zcan_frame rx_frame;

struct k_timer sync_timer;
extern void sync_function(struct k_timer *timer_id);

unsigned char torque_value[2] = {0};

void tx_irq_callback_main(uint32_t error_flags, void *arg)
{
        char *sender = (char *)arg;

        if (error_flags) {
                printk("Sendig failed [%d]\nSender: %s\n", error_flags, sender);
        }
}

void sync_function(struct k_timer *timer_id)
{

	epos4_1_pointer->sync();

	// epos4 inc. to position in rad === 0.00000756309 | 120000 inc. === 104 Grad 
	float ac_pos = (((float)(epos4_1_pointer->position))*(0.00000756309));
	float ac_tor = (((float)(epos4_1_pointer->torque)));
	if( ac_pos > 1.74532925199 ) ac_pos = 1.74532925199;
	if( ac_pos < -1.74532925199 ) ac_pos = -1.74532925199;
	Atomic_U.phi_m = -0.78539816339 - ac_pos;
	Atomic_step();


	union {
	  	int32_t float_variable;
	  	unsigned char bytes_array[4];
	} my_union;

	// Output in mNm
	//my_union.float_variable = Atomic_Y.Out1;
	my_union.float_variable = 100.0; 

	struct zcan_frame frame = {
        .id = 0x401,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 2,
        .data = {my_union.bytes_array[0],my_union.bytes_array[1]}
    };

	// printk("%d,%02x,%02x,%02x,%02x\n",my_union.float_variable,my_union.bytes_array[3],my_union.bytes_array[2],my_union.bytes_array[1],my_union.bytes_array[0]);
	printk("Data===0.0,%f,%f,%f,%f\n",ac_pos,ac_tor,Atomic_Y.Out1,Atomic_U.phi_m);
	if( 0 != can_send(epos4_1_pointer->_can_interface, &frame, K_FOREVER, tx_irq_callback_main, NULL) )
	{
		printk("\n --- Error while sending sync frame --- \n");
	}



}

void main(void)
{

	epos4 epos4_1(0x581,0x601, can_dev, &my_can_msgq1, &my_can_msgq2, &rx_frame);
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


	// Setting up the filters 

	const struct zcan_filter my_filter = {
        .id = 0x581,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .id_mask = CAN_STD_ID_MASK,
        .rtr_mask = 1
	};
	int filter_id;

	filter_id = can_attach_msgq(epos4_1_pointer->_can_interface, &my_can_msgq1, &my_filter);
	if (filter_id < 0) {
		printk("Unable to attach isr [%d]", filter_id);
		return;
	}

	const struct zcan_filter spo_recieve = {
        .id = 0x181,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .id_mask = CAN_STD_ID_MASK,
        .rtr_mask = 1
	};
	int spo_recieve_filter_id;


	spo_recieve_filter_id = can_attach_msgq(epos4_1_pointer->_can_interface, &my_can_msgq2, &spo_recieve);
	if (spo_recieve_filter_id < 0) {
		printk("Unable to attach isr [%d]", spo_recieve_filter_id);
		return;
	}


	printk(" ------------------------------- \n");
	printk("Start init ... \n");
    epos4_1_pointer->init();
	printk("\ndone!\nStart init CST-Mode ... \n");
    epos4_1_pointer->initCST();
	printk("\ndone!\n");

    k_msgq_cleanup(&my_can_msgq1);
    k_msgq_cleanup(&my_can_msgq2);

	k_sleep(K_MSEC(2000));

	Atomic_initialize();

	k_timer_init( &sync_timer, sync_function, NULL);
	k_timer_start( &sync_timer, K_MSEC(0), K_MSEC(10) );

	while(1)
	{
		//k_sleep(K_MSEC(5000));
		//epos4_1_pointer->readStatusword();
	}
}
