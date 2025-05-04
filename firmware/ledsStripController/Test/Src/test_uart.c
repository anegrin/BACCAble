#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <unistd.h>

#include "printf.h"
#include "uart_proto.h"

void _putchar(char character){
	write(1, &character, 1);
}

#define BUFFER_SIZE 26
uint8_t buffer[BUFFER_SIZE];

static void clearBuffer() {
	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0xFF;
	}
}

static void test_crc8Precalc(void **state) {
	uint8_t data[1];
	for (int i = 0; i<256; i++) {
		data[0] = i;
	    uint8_t crc = 0x00;
		crc ^= data[0];
		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x80) {
				crc = (crc << 1) ^ 0x07;
			} else {
				crc <<= 1;
			}
		}
		assert_int_equal(crc, calculate_crc8(data, 1));
	}
}



static void test_buffer(void **state, uint8_t a, uint8_t b) {
	clearBuffer();
	uint8_t data[2];
	data[0] = a;
	data[1] = b;

	fill_buffer(buffer, BUFFER_SIZE, data, 2);

	uint8_t crc = calculate_crc8(buffer, BUFFER_SIZE - 1);

	assert_int_equal(buffer[0], a);
	assert_int_equal(buffer[1], b);
	for (int i = 2; i < BUFFER_SIZE - 1; i++) {
		assert_int_equal(buffer[i], 0xFF);
	}
	assert_int_equal(buffer[BUFFER_SIZE - 1], crc);
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
	clearBuffer();
	uint8_t data[5];
    data[0] = BhBusIDparamString;
    data[1] = 'H';
    data[2] = 'e';
    data[3] = 'y';
    data[4] = '!';

	fill_buffer(buffer, BUFFER_SIZE, data, 5);

	uint8_t crc = calculate_crc8(buffer, BUFFER_SIZE - 1);

	assert_int_equal(buffer[0], BhBusIDparamString);
	assert_int_equal(buffer[1], 'H');
	assert_int_equal(buffer[2], 'e');
	assert_int_equal(buffer[3], 'y');
	assert_int_equal(buffer[4], '!');
	for (int i = 5; i < BUFFER_SIZE - 1; i++) {
		assert_int_equal(buffer[i], 0xFF);
	}
	assert_int_equal(buffer[BUFFER_SIZE - 1], crc);
}

static void test_validateRxBuffer(void **state, uint8_t a, uint8_t b, uint8_t crc, bool valid) {
	clearBuffer();
	uint8_t data[2];
	data[0] = a;
	data[1] = b;

	fill_buffer(buffer, BUFFER_SIZE, data, 2);
	buffer[BUFFER_SIZE - 1] = crc;
/*
	for (int i = 0; i<BUFFER_SIZE; i++) printf("%x,", buffer[i]);
	printf("\n<%d vs %d>\n", calculate_crc8(buffer, BUFFER_SIZE - 1), crc);
*/

	assert_int_equal(valid, validateRxBuffer(buffer, BUFFER_SIZE));
}

static void test_validateRxBufferC1(void **state) {
	(void) state; // unused
	test_validateRxBuffer(state, C1BusID, C2cmdNormalFrontBrake, 234, true);
	test_validateRxBuffer(state, C1BusID, C2cmdForceFrontBrake, 128, true);
	test_validateRxBuffer(state, C1BusID, C2cmdForceFrontBrake, 145, false);
	test_validateRxBuffer(state, C1BusID, C2cmdtoggleDyno, 128, false);
}

static void test_validateRxBufferC2(void **state) {
	(void) state; // unused
	test_validateRxBuffer(state, C2BusID, C2cmdtoggleDyno, 221, true);
	test_validateRxBuffer(state, C2BusID, C2cmdNormalFrontBrake, 251, true);
	test_validateRxBuffer(state, C2BusID, C2cmdForceFrontBrake, 145, true);
	test_validateRxBuffer(state, C2BusID, C2cmdGetStatus, 183, true);
	test_validateRxBuffer(state, C2BusID, C2cmdtoggleEscTc, 69, true);
	test_validateRxBuffer(state, C2BusID, C2cmdForceFrontBrake, 128, false);
	test_validateRxBuffer(state, C2BusID, 'A', 152, false);
}

int main(void) {
    const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_crc8Precalc),
		cmocka_unit_test(test_bufferC2BusID),
		cmocka_unit_test(test_bufferC1BusID),
		cmocka_unit_test(test_bufferBHBusID),
		cmocka_unit_test(test_validateRxBufferC1),
		cmocka_unit_test(test_validateRxBufferC2),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
