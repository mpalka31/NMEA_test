/**
 * @file	uart-comport.c
 * @author 	Michał Pałka
 *
 * Implementation of uart-comport communication driver.
 *
 */
#include "uart-comport.h"
#include <string.h>
#define COM_LEN		9

static uint8_t comand_RX[COM_LEN]={0};	/**< UART Rx buffer*/

static bool DAV_flag = false;	/**< data valid flag. When true, uart_comport_task will triggered task() function.*/

void uatr_comport_comunication_init(UART_HandleTypeDef *huart){
	com_huart = huart;
	HAL_UART_Receive_IT(com_huart, comand_RX, COM_LEN);
}

void uatr_comport_read_data(){
	DAV_flag = true;
	for (int i=0; i<3; i++){
		start_speed[i]=comand_RX[i];
	}
	for (int i=0; i<3; i++){
		stop_speed[i]=comand_RX[i+4];
	}
	HAL_UART_Receive_IT(com_huart, comand_RX, COM_LEN);
}

void uatr_comport_send_data(uint8_t* data, uint8_t data_len){
	HAL_UART_Transmit_IT(com_huart, data, data_len);
}

void uart_comport_task(void (*task)(void)){
	if (DAV_flag){
		task();
		DAV_flag=false;
	}
}
