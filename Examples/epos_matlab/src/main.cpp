#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>
#include <sys/byteorder.h>
#include <stdio.h>
#include <math.h>

#ifdef CONFIG_UART_ASYNC_API
#include <zephyr/drivers/dma/dma_stm32.h>
#include <zephyr/drivers/dma.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#include "Control_System.h"
#ifdef __cplusplus
}
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
struct k_mutex blocker_can;

extern void sync_function(struct k_timer *timer_id);

uint32_t glob_counter = 0;
int glob_counter_int = 0;

uint8_t uart_data[100];
volatile float final_pos = 100.0;

/**
 * @brief Predefined trajectory to run down. 
 * 
 * @details At any point in time, the value of the trajectory can be returned.
 */
float get_trejectory(int glob_counter, int mode)
{
	float final_pos = 0.0;
	if( mode == 0 ){

		// Two jumps. Start at 0 degrees. Then drive to 50 degrees and then back to 30 degrees.
		if( glob_counter >= 0 && glob_counter < 3000 ){
			final_pos = 0.0;
		}else if ( glob_counter >= 3000 && glob_counter < 10000 ){
			final_pos = 50.0;
		}else if ( glob_counter >= 10000 ){
			final_pos = 30.0;
		} else {
			final_pos = 0.0;
		}

	}else if( mode == 1 ){

		// Preset trejectory with ramps
		if( glob_counter >= 0 && glob_counter < 5000 ){
			final_pos = 0.0;
		}else if ( glob_counter >= 5000 && glob_counter < 10000 ){
			final_pos = (50.0/5000.0)*(glob_counter - 5000.0) + 0.0;
		}else if ( glob_counter >= 10000 && glob_counter < 15000 ){
			final_pos = (50.0/5000.0)*(glob_counter - 10000.0) + 50.0;
		}else if ( glob_counter >= 15000 && glob_counter < 20000 ){
			final_pos = (-40.0/5000.0)*(glob_counter - 15000.0) + 100.0;
		}else if ( glob_counter >= 20000 && glob_counter < 25000 ){
			final_pos = (-30.0/5000.0)*(glob_counter - 20000.0) + 60.0;
		}else if ( glob_counter >= 25000 && glob_counter < 30000 ){
			final_pos = (20.0/5000.0)*(glob_counter - 25000.0) + 30.0;
		}else if ( glob_counter >= 30000 ){
			final_pos = 50.0;
		}else{
			final_pos = 0.0;
		}

	}else{

		final_pos = 0.0;

	}
	return final_pos;
}

/**
 * @brief UART data format 
 * 
 * @details So that measurement data can be sent to the computer via uart, the measurement data is put into this stupid format. 
 * This way the measured values can be distinguished from other uart data.
 */
void save_data_for_uart(uint8_t *uart_data, uint8_t *time_pointer, uint8_t *pos_pointer, int8_t *pointer_tor, int8_t *final_pos_pointer, int buffer_part)
{
	uart_data[(buffer_part)] = 'D';
	uart_data[(buffer_part)+1] = '=';
	uart_data[(buffer_part)+2] = 'x';
	uart_data[(buffer_part)+3] = time_pointer[0];
	uart_data[(buffer_part)+4] = time_pointer[1];
	uart_data[(buffer_part)+5] = time_pointer[2];
	uart_data[(buffer_part)+6] = time_pointer[3];
	uart_data[(buffer_part)+7] = 'x';
	uart_data[(buffer_part)+8] = '=';
	uart_data[(buffer_part)+9] = pos_pointer[0];
	uart_data[(buffer_part)+10] = pos_pointer[1];
	uart_data[(buffer_part)+11] = pos_pointer[2];
	uart_data[(buffer_part)+12] = pos_pointer[3];
	uart_data[(buffer_part)+13] = 'x';
	uart_data[(buffer_part)+14] = '=';
	uart_data[(buffer_part)+15] = final_pos_pointer[0];
	uart_data[(buffer_part)+16] = final_pos_pointer[1];
	uart_data[(buffer_part)+17] = final_pos_pointer[2];
	uart_data[(buffer_part)+18] = final_pos_pointer[3];
	uart_data[(buffer_part)+19] = 'x';
	uart_data[(buffer_part)+20] = '=';
	uart_data[(buffer_part)+21] = pointer_tor[0];
	uart_data[(buffer_part)+22] = pointer_tor[1];
	uart_data[(buffer_part)+23] = '\r';
	uart_data[(buffer_part)+24] = '\n';
}

void tx_irq_callback_main(const struct device *CAN_INTERFACE, int error_flags, void *arg)
{
    k_mutex_unlock(epos4_1_pointer->_blocker_can);
}

void sync_function(struct k_timer *timer_id)
{ 	
	// ----------------------------------------------------------------------
	// Einlesen der Aktuellen Position und Geschwindigkeit des Aktuators. 
	// Position ist in Encode increments und Geschwindigkeit in rpm.
	int32_t tmp_pos = epos4_1_pointer->position;
	int32_t tmp_vel = epos4_1_pointer->velocity;

	// ----------------------------------------------------------------------
	// Send synconization signal (once a millisecond) to the EPOS. 
	// This expects it once per time interval (1ms). 
	// Then it sends previously agreed data (SPO Object. position and speed). 
	epos4_1_pointer->sync();
	
	// ----------------------------------------------------------------------
	// Conversion of position and speed in radian and rad/s
	// 		1 rad == 0.00000756309 inc. 
	//		104 deg == 120000 inc.
	float ac_pos = (((float)(tmp_pos)) * 0.00001512618 + 1.5707963267);
	float ac_vel = (((float)(tmp_vel)) * 0.00065797516 );

	// ----------------------------------------------------------------------
	// Defining the next position
	final_pos = get_trejectory(glob_counter, 0);
	
	// ----------------------------------------------------------------------
	// Here we use a code generated Simulink model (position controller) to calculate the new Controlling variable. 
	// If no Simulink model is used, the new value must be calculated in another way. 

	// Store the measured values (position and velocity) and the default position on the Simulink object of the mathematical model. 
	Control_System_U.vel_meas_rads = ac_vel;
	Control_System_U.pos_meas_rad = ac_pos;
	Control_System_U.User_Posdeg = final_pos;

	// Some constants
	Control_System_U.Unlock_Motor = 1;
	Control_System_U.User_ControlMode = 1;
	Control_System_U.torque_meas_Nm = 0.0;

	// ----------------------------------------------------------------------
	// Perform a simulation step in the Simulink model (controller). 
	// This calculates the new motor current/torque.
	// the new value is stored in >Control_System_Y.Output_Torque<
	Control_System_step();

	// ----------------------------------------------------------------------
	// Save the torque calculated by the controller to send it to the EPOS4. 
	float target_tor = Control_System_Y.Output_Torque * 1000.0;
	int8_t *tar_tor_array = (int8_t*)(&target_tor);
	
	// ----------------------------------------------------------------------
	// conversion from signed int to unsigned int. 
	// This allows the data to be sent directly as a byte via UART. 
	uint32_t int_pos = (uint32_t)(tmp_pos);
	uint32_t int_tor = (uint32_t)(tmp_vel);

	// ----------------------------------------------------------------------
	// Change all variables to the correct data types for further processing. 
	// Send via CAN to the current controller (EPOS4) and send via UART to the computer for graphical output. 
	int16_t int16_tor = (int16_t)(target_tor);
	int32_t int32_final_pos = (int32_t)(final_pos);

	uint8_t *time_pointer = (uint8_t *)(&glob_counter);
	uint8_t *pos_pointer = (uint8_t *)(&int_pos);
	uint8_t *tor_pointer = (uint8_t *)(&int_tor);
	int8_t *pointer_tor = (int8_t *)(&int16_tor);
	int8_t *final_pos_pointer = (int8_t *)(&int32_final_pos);

	// ----------------------------------------------------------------------
	// Sending the data via UART. 
	// First the data is written into a buffer. We use a DMA to send the data via UART. 
	// With this we can save the UART send oberhead at every send call and send directly larger amounts of data. 
	// We send at every fourth pass (every 4 ms) the measurement data of the last 4 passes. 
	int buffer_part_mod = glob_counter%4;

	// save the data on the predefined buffer
	save_data_for_uart(uart_data, time_pointer, pos_pointer, pointer_tor, final_pos_pointer, buffer_part_mod*25);

	// When the buffer is full or every 4ms the buffer is passed to the DMA. 
	// This then sends it asynchronously to the uart port.
	if( buffer_part_mod == 3 ){
		uart_tx(uart_dev, uart_data, 100, 0);
	}

	// Create a CAN frame.
	// This is sent to the EPOS4 and contains the next position to be approached. 
	struct zcan_frame frame = {
        .id = 0x401,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 2,
        .data = {pointer_tor[0],pointer_tor[1]}
    };

	// Wait until the last can send function has returned to avoid a deadlock. Known bug from the CAN library of zephyr
    k_mutex_lock(epos4_1_pointer->_blocker_can, K_FOREVER);

	// ----------------------------------------------------------------------
	// Send the CAN frame
	int can_send_return = can_send(epos4_1_pointer->_can_interface, &frame, K_FOREVER, tx_irq_callback_main, NULL);
	
	// increase of the cycle counter
	glob_counter += 1;
	glob_counter_int += 1;
	
	// ----------------------------------------------------------------------
	// General possibility to perform a time measurement. 
	// The number of cycles must be multiplied with the clock rate of the controller to calculate the execution time.
	// uint32_t start_time = k_cycle_get_32();
	// uint32_t end_time = k_cycle_get_32();
	// uint32_t diff_time = end_time - start_time;
}

void serial_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	
}

void rx_callback_function(const struct device *dev, struct zcan_frame *frame, void *user_data)
{
    epos4_1_pointer->reciveCANdataSPO((char*)(frame->data));
}

void main(void)
{
	// --------------------------------------
	// Complete init of the CAN controller and the UART device. 

	// init uart dev	
	if ( !device_is_ready(uart_dev) ) {
		printk("uart devices not ready\n");
		return;
	}
	uart_callback_set(uart_dev, serial_cb, NULL);
	
	// init the Epos instance and the CAN interface for it.
	epos4 epos4_1(0x581,0x601, can_dev, &my_can_msgq1, &my_can_msgq2, &rx_frame, &blocker_can);
	epos4_1_pointer = &epos4_1;
	epos4_1_pointer->_can_interface = device_get_binding("CAN_1");
	epos4_1_pointer->measure_mode = 0;

    if (!device_is_ready(epos4_1_pointer->_can_interface)) {
		printk("CAN: Device %s not ready.\n", epos4_1_pointer->_can_interface->name);
		return;
	}else{
        printk("CAN device %s is ready...\n", epos4_1_pointer->_can_interface->name);
    }
	can_set_mode(epos4_1_pointer->_can_interface, CAN_MODE_NORMAL);

	// Due to a known bug in the Zephyr CAN interface, we need a mutex to protect the CAN send function. This is initialized here.
    k_mutex_init(epos4_1_pointer->_blocker_can);

	// Setting up the filters for receiving CAN msgs. 
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
	// Program init finished
	// -------------------------------------------

	k_sleep(K_MSEC(500));

	// -------------------------------------------
	// Init the Epos4 Controller

	printk(" ------------------------------- \n");
	printk("Start init ... \n");
    epos4_1_pointer->init();
	printk("\ndone!\nStart init CST-Mode ... \n");
    epos4_1_pointer->initCST();
	printk("\ndone!\n");

    k_msgq_cleanup(&my_can_msgq1);
    k_msgq_cleanup(&my_can_msgq2);

	// Epos4 init finished
	// -------------------------------------------

	k_sleep(K_MSEC(1000));

	
	// Init Matlab code model
	Control_System_initialize();

	// Initialize and start the interrupt routines to call the sync function regularly. 
	// There happens the main routine of the program
	k_timer_init( &sync_timer, sync_function, 0);
	k_timer_start( &sync_timer, K_MSEC(0), K_MSEC(1) );
	
	while(1)
	{
		// ...
	}
}
