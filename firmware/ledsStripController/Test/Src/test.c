#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <unistd.h>
#include "dashboard.h"

extern const int _DASHBOARD_SIZE_OF_FLOAT;
extern const DashboardItem FIRMWARE_ITEM;
extern const DashboardItem HP_ITEM;
extern const DashboardItem FRONT_TIRES_TEMP_ITEM;

void _putchar(char character){
	write(1, &character, 1);
}

void floatToBytes(float val, uint8_t* bytes) {
    memcpy(bytes, &val, _DASHBOARD_SIZE_OF_FLOAT);
}

static void test_encodeToMessage(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;
	uint8_t itemId = 0x42;
	float value1 = 12.34f;
	float value2 = 56.78f;

	// Call the function under test
	encodeToMessage(buffer, offset, itemId, value1, value2);

	// Check that itemId is written correctly
	assert_int_equal(buffer[offset], itemId);

	// Check the float values
	uint8_t expectedValue1[4];
	uint8_t expectedValue2[4];
	floatToBytes(value1, expectedValue1);
	floatToBytes(value2, expectedValue2);

	assert_memory_equal(&buffer[offset + 1], expectedValue1, _DASHBOARD_SIZE_OF_FLOAT);
	assert_memory_equal(&buffer[offset + 1 + _DASHBOARD_SIZE_OF_FLOAT], expectedValue2, _DASHBOARD_SIZE_OF_FLOAT);
}

static void test_decodeToItemLabelNoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, 0x42, 12.34f, -1);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, FIRMWARE_ITEM.id, labelBuffer, 18);

	assert_string_equal(labelBuffer, "BACCAble 3.0");

}

static void test_decodeToItemLabelOneValue(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, 0x42, 12.34f, -1);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, HP_ITEM.id, labelBuffer, 18);

	assert_string_equal(labelBuffer, "Power: 12hp");

}

static void test_decodeToItemLabelTwoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, 0x42, 12.34f, 56.78f);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, FRONT_TIRES_TEMP_ITEM.id, labelBuffer, 18);

	assert_string_equal(labelBuffer, "12°C FT 57°C");// 2nd value is rounded

}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_encodeToMessage),
	        cmocka_unit_test(test_decodeToItemLabelNoValues),
	        cmocka_unit_test(test_decodeToItemLabelOneValue),
	        cmocka_unit_test(test_decodeToItemLabelTwoValues),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
