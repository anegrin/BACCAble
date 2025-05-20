#ifndef _UART_H
#define _UART_H

	#include <stdbool.h>
	#include "stm32f0xx_hal.h"


	#include "uart_proto.h"
	#include "onboardLed.h"
	#include "string.h"
	//#include "error.h"

	#define UART_BUFFER_SIZE DASHBOARD_BUFFER_SIZE + 1


	
	void uart_init();
	//void uart_transmit_data(char*  message);
	void process_received_data();
	void enter_standby_mode();
	void resetOtherProcessorsSleepStatus();
	uint8_t getOtherProcessorsSleepingStatus();
	void addToUARTSendQueue(const uint8_t *data, size_t length);
	void processUART();
#endif
