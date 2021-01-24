/**
 * @file	uart-comport.h
 * @author 	Michał Pałka
 *
 * <pre>
 * This is include file for uart-comport communication driver.
 * It is used for receiving measurement configuration commands.
 * Every command has to contain 9 characters.
 *
 * PROTOCOL:
 * "XXX YYY\t\n"
 *
 * XXX -	start measurement speed
 * YYY -	stop measurement speed
 *
 * When XXX<YYY then it triggered the measurement
 * e.g. "010 060\t\n" will triggered measurement of acceleration from 10 to 60 kmpk and "measurement accepted\n" will be send to the comport.
 *
 * When XXX==YYY then it cancel the measurement
 * e.g. "000 000\t\n" will cancelled measurement and "measurement cancelled\n" will be send to the comport.
 *
 * When XXX>YYY nothing happened
 * e.g. "100 000\t\n" the "stop 0 less then start 100\n" will be send to the comport.
 * </pre>
 */
#include "stm32l4xx_hal.h"
#include <stdbool.h>
/**
 * Pointer to UART handler structure used to communicate with comport.
 */
UART_HandleTypeDef	*com_huart;

uint8_t start_speed[3];	/**< buffer for start measurement speed*/
uint8_t stop_speed[3];	/**< buffer for stop measurement speed*/
/**
 * uatr_comport_comunication_init is a function to initialize connection
 * @param[in]	huart	is a pointer to UART_HandleTypeDef used to communicate with comport.
 */
void uatr_comport_comunication_init(UART_HandleTypeDef *huart);
/**
 * uatr_comport_read_data is a function used to read data from Rx buffer to start_speed and stop_speed.
 */
void uatr_comport_read_data(void);
/**
 * uatr_comport_send_data is a function used to send message to comport. It sends data in non-blocking mode.
 * @param[in]	data		is a pointer to sending data
 * @param[in]	data_len	is length of data to send
 *
 * It should be placed inside:
 * @code
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) uatr_comport_read_data();
}
 * @endcode
 */
void uatr_comport_send_data(uint8_t* data, uint8_t data_len);
/**
 * uart_comport_task is a function that should be placed inside main infinitive loop.
 * @param[in]	task	is a function pointer to a function which shoud be triggered to handle message. It is implemented in the "main.c" file.
 */
void uart_comport_task(void (*task)(void));
