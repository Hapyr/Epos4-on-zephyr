#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/can.h>
#include <drivers/gpio.h>
#include <sys/byteorder.h>

#define RX_THREAD_STACK_SIZE 512
#define RX_THREAD_PRIORITY 2
#define STATE_POLL_THREAD_STACK_SIZE 512
#define STATE_POLL_THREAD_PRIORITY 2
#define LED_MSG_ID 0x10
#define COUNTER_MSG_ID 0x12345
#define SET_LED 1
#define RESET_LED 0
#define SLEEP_TIME K_MSEC(250)

struct k_thread rx_thread_data;
struct k_thread poll_state_thread_data;
struct zcan_work rx_work;
struct k_work state_change_work;
enum can_state current_state;
struct can_bus_err_cnt current_err_cnt;


// const struct device *can_dev = DEVICE_DT_GET(DT_ALIAS(can_dev));
#define CAN_DEVICE_LABEL DT_PROP(DT_ALIAS(can_dev), label)


void main(void)
{
	k_tid_t rx_tid;

	struct zcan_frame frame = {
        .id = 0x601,
        .rtr = CAN_DATAFRAME,
        .id_type = CAN_STANDARD_IDENTIFIER,
        .dlc = 8,
        .data = {0x40, 0x18, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00}
	};
	int ret;

	const struct device* can_dev = device_get_binding("CAN_1");

    if (!device_is_ready(can_dev)) {
		printk("CAN: Device %s not ready.\n", can_dev->name);
		return;
	}else{
        printk("CAN device %s is ready...\n", can_dev->name);
    }

	can_set_mode(can_dev, CAN_NORMAL_MODE);
	// can_set_mode(can_dev, CAN_LOOPBACK_MODE);

	while(1)
	{
		k_sleep(K_MSEC(1000));
		

		ret = can_send(can_dev, &frame, K_MSEC(1000), NULL, NULL);
		if (ret != CAN_TX_OK) {
		        printk("Sending failed [%d]\n", ret);
		} else {
			printk("DID it :)\n");
		}
	}
}