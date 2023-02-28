#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>
#include <sys/byteorder.h>
#include <stdio.h>

#ifdef CONFIG_UART_ASYNC_API
#include <zephyr/drivers/dma/dma_stm32.h>
#include <zephyr/drivers/dma.h>
#endif

#include "epos4.h"

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

// uart config
#define UART_DEVICE_NODE DT_ALIAS(usart2)
const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

epos4* epos4_1_pointer = NULL;

struct k_thread rx_thread_data;
struct k_thread poll_state_thread_data;
struct k_work state_change_work;
enum can_state current_state;
struct can_bus_err_cnt current_err_cnt;


CAN_MSGQ_DEFINE(my_can_msgq1, 100);
CAN_MSGQ_DEFINE(my_can_msgq2, 100);
struct zcan_frame rx_frame;

struct k_timer sync_timer;
struct k_timer print_timer;
extern void sync_function(struct k_timer *timer_id);

unsigned char torque_value[2] = {0};

int counter_1 = 0;
bool count_checker = true;
uint32_t diff;
uint8_t send_data[2000];
uint8_t send_data_tmp[100];
uint32_t glob_counter = 0;
int glob_counter_int = 0;

int32_t vvaa = 100;

struct k_mutex blocker_can;



void tx_irq_callback_main(const struct device *CAN_INTERFACE, int error_flags, void *arg)
{
    k_mutex_unlock(epos4_1_pointer->_blocker_can);
    // char *sender = (char *)arg;
	// 
    // if (error_flags) {
    //     printk("Sendig main [%d]\nSender: %s\n", error_flags, sender);
    // }
}
void sync_function(struct k_timer *timer_id)
{ 	
	// Overwrite
	int32_t tmp_pos = epos4_1_pointer->position;
	int32_t tmp_tor = epos4_1_pointer->torque;

	epos4_1_pointer->sync();
	
	
	// epos4 inc. to position in rad === 0.00000756309 | 120000 inc. === 104 Grad 
	float ac_pos = (((float)(tmp_pos))*(0.00000756309));
	float ac_tor = (((float)(tmp_tor)));
	
	uint32_t int_pos = (uint32_t)(epos4_1_pointer->position);
	// uint32_t int_tor = (uint32_t)(epos4_1_pointer->velocity);
	uint16_t int_tor = (uint16_t)(epos4_1_pointer->torque);
	
	uint8_t *time_pointer;
	uint8_t *pos_pointer;
	uint8_t *tor_pointer;

	time_pointer = (uint8_t *)(&glob_counter);
	pos_pointer = (uint8_t *)(&int_pos);
	tor_pointer = (uint8_t *)(&int_tor);

	int16_t int16_tor = 0;
	int8_t *pointer_tor;

	if( glob_counter_int >= 0 && glob_counter_int < 1000 ) int16_tor = 0;
	if( glob_counter_int >= 1000 && glob_counter_int < 2000 ) int16_tor = 200;
	if( glob_counter_int >= 2000 && glob_counter_int < 4000 ) int16_tor = 0;
	if( glob_counter_int >= 4000 && glob_counter_int < 4500 ) int16_tor = 300;
	if( glob_counter_int >= 5000 && glob_counter_int < 5500 ) int16_tor = 0;
	if( glob_counter_int >= 5500 && glob_counter_int < 5900 ) int16_tor = -100;
	if( glob_counter_int >= 5900 && glob_counter_int < 10000 ) int16_tor = 0;

	if( glob_counter_int >= 10000 && glob_counter_int < 11000 ) int16_tor = 200;
	if( glob_counter_int >= 11000 && glob_counter_int < 11500 ) int16_tor = 0;
	if( glob_counter_int >= 11500 && glob_counter_int < 12000 ) int16_tor = -100;
	if( glob_counter_int >= 12000 && glob_counter_int < 12500 ) int16_tor = 0;
	if( glob_counter_int >= 12500 && glob_counter_int < 14500 ) int16_tor = 150;
	if( glob_counter_int >= 14500 && glob_counter_int < 20000 ) int16_tor = 0;
	if( glob_counter_int >= 20000 || glob_counter_int < 0 ) glob_counter_int = 0;

	pointer_tor = (int8_t *)(&int16_tor);

	struct zcan_frame frame = {
        .id = 0x401,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 2,
        .data = {pointer_tor[0],pointer_tor[1]}
        // .data = {0x0,0x0}
    };

	int tester = glob_counter%4;
	int tester_1 = tester*25;

	send_data_tmp[(tester_1)] = 'D';
	send_data_tmp[(tester_1)+1] = '=';
	send_data_tmp[(tester_1)+2] = 'x';
	send_data_tmp[(tester_1)+3] = time_pointer[0];
	send_data_tmp[(tester_1)+4] = time_pointer[1];
	send_data_tmp[(tester_1)+5] = time_pointer[2];
	send_data_tmp[(tester_1)+6] = time_pointer[3];
	send_data_tmp[(tester_1)+7] = 'x';
	send_data_tmp[(tester_1)+8] = '=';
	send_data_tmp[(tester_1)+9] = pos_pointer[0];
	send_data_tmp[(tester_1)+10] = pos_pointer[1];
	send_data_tmp[(tester_1)+11] = pos_pointer[2];
	send_data_tmp[(tester_1)+12] = pos_pointer[3];
	send_data_tmp[(tester_1)+13] = 'x';
	send_data_tmp[(tester_1)+14] = '=';
	send_data_tmp[(tester_1)+15] = tor_pointer[0];
	send_data_tmp[(tester_1)+16] = tor_pointer[1];
	send_data_tmp[(tester_1)+17] = 0x0;
	send_data_tmp[(tester_1)+18] = 0x0;
	send_data_tmp[(tester_1)+19] = 'x';
	send_data_tmp[(tester_1)+20] = '=';
	send_data_tmp[(tester_1)+21] = pointer_tor[0];
	send_data_tmp[(tester_1)+22] = pointer_tor[1];
	send_data_tmp[(tester_1)+23] = '\r';
	send_data_tmp[(tester_1)+24] = '\n';
		
	
	// my_union.float_variable = 0;

	if( tester == 3 ){
		uart_tx(uart_dev, send_data_tmp, 100, 0);
	}

	// printk("%d,%02x,%02x,%02x,%02x\n",my_union.float_variable,my_union.bytes_array[3],my_union.bytes_array[2],my_union.bytes_array[1],my_union.bytes_array[0]);
	// printk("Data===0.0,%f,%f+\n",ac_pos,ac_tor);
	
	// if( 0 != can_send(epos4_1_pointer->_can_interface, &frame, K_FOREVER, tx_irq_callback_main, NULL) )

    k_mutex_lock(epos4_1_pointer->_blocker_can, K_FOREVER);

	uint32_t start_time = k_cycle_get_32();
	int can_send_return = can_send(epos4_1_pointer->_can_interface, &frame, K_FOREVER, tx_irq_callback_main, NULL);
	//int can_send_return = 0;
	uint32_t end_time = k_cycle_get_32();
	

	if(can_send_return != 0){
		//uint32_t asdf = (uint32_t)(can_send_return);
		//uint8_t *asdf_t;
		//asdf_t = (uint8_t *)(&asdf);
		//
		//uint8_t err_msg[5] = {asdf_t[0],asdf_t[1],asdf_t[2],asdf_t[3],'\n'};
		//uart_tx(epos4_1_pointer->_can_interface, err_msg, 5, 0);
	}
	glob_counter += 1;
	glob_counter_int += 1;
}
void serial_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
}
void rx_callback_function(const struct device *dev, struct zcan_frame *frame, void *user_data)
{
	
	// uint8_t err_msg[2] = {'S','\n'};
	// uart_tx(uart_dev, err_msg, 2, 0);
    epos4_1_pointer->reciveCANdataSPO((char*)(frame->data));
}

void main(void)
{
	// init uart dev	
	if ( !device_is_ready(uart_dev) ) {
		printk("uart devices not ready\n");
		return;
	}

	uart_callback_set(uart_dev, serial_cb, NULL);

	epos4 epos4_1(0x581,0x601, can_dev, &my_can_msgq1, &my_can_msgq2, &rx_frame, &blocker_can);
	epos4_1_pointer = &epos4_1;
	epos4_1_pointer->_can_interface = device_get_binding("CAN_1");

    if (!device_is_ready(epos4_1_pointer->_can_interface)) {
		printk("CAN: Device %s not ready.\n", epos4_1_pointer->_can_interface->name);
		return;
	}else{
        printk("CAN device %s is ready...\n", epos4_1_pointer->_can_interface->name);
    }
	can_set_mode(epos4_1_pointer->_can_interface, CAN_MODE_NORMAL);

	
    k_mutex_init(epos4_1_pointer->_blocker_can);

	// Setting up the filters 

	const struct zcan_filter my_filter = {
        .id = 0x581,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .id_mask = CAN_STD_ID_MASK,
        .rtr_mask = 1
	};
	int filter_id;

	filter_id = can_add_rx_filter_msgq(epos4_1_pointer->_can_interface, &my_can_msgq1, &my_filter);
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

	spo_recieve_filter_id = can_add_rx_filter(epos4_1_pointer->_can_interface, rx_callback_function, NULL, &spo_recieve);
	// spo_recieve_filter_id = can_add_rx_filter_msgq(epos4_1_pointer->_can_interface, &my_can_msgq2, &spo_recieve);
	if (spo_recieve_filter_id < 0) {
		printk("Unable to attach isr [%d]", spo_recieve_filter_id);
		return;
	}

	k_sleep(K_MSEC(500));

	printk(" ------------------------------- \n");
	printk("Start init ... \n");
    epos4_1_pointer->init();
	printk("\ndone!\nStart init CST-Mode ... \n");
    epos4_1_pointer->initCST();
	printk("\ndone!\n");




    k_msgq_cleanup(&my_can_msgq1);
    k_msgq_cleanup(&my_can_msgq2);

	k_sleep(K_MSEC(1000));

	k_timer_init( &sync_timer, sync_function, NULL);
	k_timer_start( &sync_timer, K_MSEC(0), K_MSEC(1) );
	

	// k_timer_init( &print_timer, print_function, NULL);
	// k_timer_start( &print_timer, K_MSEC(0), K_MSEC(100) );

	while(1)
	{
		//k_sleep(K_MSEC(5000));
		//epos4_1_pointer->readStatusword();
	}
}
