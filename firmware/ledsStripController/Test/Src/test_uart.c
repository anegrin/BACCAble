#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <unistd.h>

#include "printf.h"
#include "uart_helper.h"

void _putchar(char character){
	write(1, &character, 1);
}

#define C1BusID					0x01 //first byte sent over uart to identify destinator baccable connected to C1 Can Bus
#define C2BusID					0x02 //first byte sent over uart to identify destinator baccable connected to C2 Can Bus
#define BhBusIDparamString		0x03 //first byte sent over uart to identify destinator baccable connected to BH Can Bus to transfer parameter string
#define AllSleep				0x04 //first byte sent over uart to tell anyone connecter to go to sleep (or low consumption).
#define C2BusIDAllSleepAck		0x05 //first byte sent over uart by C2 baccable in order to communicate that the sleep was received and executed
#define BHBusIDAllSleepAck		0x06 //first byte sent over uart by BH baccable in order to communicate that the sleep was received and executed
#define AllResetFaults			0x07 //first byte sent over uart by C1 baccable in order to communicate that the Reset Faults is required
#define BhBusIDgetStatus		0x08 //first byte sent over uart to identify destinator baccable connected to BH Can Bus to request its status
#define BhBusChimeRequest		0x09 //first byte sent over uart to indentify message to BH to play sound


#define C2cmdtoggleDyno			0x20 //second byte of the message to C2 bus, identifies the request to toggle dyno
#define C2cmdNormalFrontBrake	0x21 //second byte of the message to C2 bus, identifies the request to set front Brake to normal
#define C2cmdForceFrontBrake	0x22 //second byte of the message to C2 bus, identifies the request to force Front Brake ON
#define C2cmdGetStatus			0x23 //second byte of the message to C2 bus, identifies the request to getStatus
#define C2cmdtoggleEscTc		0x24 //second byte of the message to C2 bus, identifies the request to toggle ESC/TC

#define UART_BUFFER_SIZE 20	
uint8_t tx_buffer[UART_BUFFER_SIZE];

static void test_buffer(void **state, uint8_t a, uint8_t b) {
	uint8_t data[2];
	data[0] = a;
	data[1] = b;
	
	fill_buffer(tx_buffer, UART_BUFFER_SIZE, data, 2);
	
	uint8_t crc = calculate_crc8(tx_buffer, UART_BUFFER_SIZE - 1);
	
	assert_int_equal(tx_buffer[0], a);
	assert_int_equal(tx_buffer[1], b);
	for (int i = 2; i < UART_BUFFER_SIZE - 1; i++) {
		assert_int_equal(tx_buffer[i], 0xFF);
	}
	assert_int_equal(tx_buffer[UART_BUFFER_SIZE - 1], crc);
}

static void test_bufferC2BusID(void **state) {
	(void) state; // unused
	test_buffer(state, C2BusID, C2cmdtoggleDyno);
	test_buffer(state, C2BusID, C2cmdNormalFrontBrake);
	test_buffer(state, C2BusID, C2cmdForceFrontBrake);
	test_buffer(state, C2BusID, C2cmdGetStatus);
	test_buffer(state, C2BusID, C2cmdtoggleEscTc);
}

static void test_bufferC1BusID(void **state) {
	(void) state; // unused
	test_buffer(state, C1BusID, C2cmdNormalFrontBrake);
	test_buffer(state, C1BusID, C2cmdForceFrontBrake);
}

static void test_bufferBHBusID(void **state) {
	(void) state; // unused
	uint8_t data[5];
    data[0] = BhBusIDparamString;
    data[1] = 'H';
    data[2] = 'e';
    data[3] = 'y';
    data[4] = '!';
	
	fill_buffer(tx_buffer, UART_BUFFER_SIZE, data, 5);
	
	uint8_t crc = calculate_crc8(tx_buffer, UART_BUFFER_SIZE - 1);
	
	assert_int_equal(tx_buffer[0], BhBusIDparamString);
	assert_int_equal(tx_buffer[1], 'H');
	assert_int_equal(tx_buffer[2], 'e');
	assert_int_equal(tx_buffer[3], 'y');
	assert_int_equal(tx_buffer[4], '!');
	for (int i = 5; i < UART_BUFFER_SIZE - 1; i++) {
		assert_int_equal(tx_buffer[i], 0xFF);
	}
	assert_int_equal(tx_buffer[UART_BUFFER_SIZE - 1], crc);
}

int main(void) {
    const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_bufferC2BusID),
		cmocka_unit_test(test_bufferC1BusID),
		cmocka_unit_test(test_bufferBHBusID),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}