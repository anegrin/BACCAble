#ifndef _UART_HELPER_H
#define _UART_HELPER_H
#include <stdint.h>
#include <string.h>

void fill_buffer(uint8_t *buffer, uint8_t bufferLength, uint8_t *data, uint8_t dataLength);
uint8_t calculate_crc8(uint8_t *data, uint8_t length);

#endif
