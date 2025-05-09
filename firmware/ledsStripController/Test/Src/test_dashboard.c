#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <unistd.h>

#include "dashboard.h"

void _putchar(char character){
	write(1, &character, 1);
}

void floatToBytes(float val, uint8_t* bytes) {
    memcpy(bytes, &val, DASHBOARD_SIZE_OF_FLOAT);
}

static void test_encodeToDashboardMessage(void **state) {
	(void) state; // unused

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;
	uint8_t itemId = 0x42;
	float value1 = 12.34f;
	float value2 = 56.78f;

	// Call the function under test
	encodeToDashboardMessage(buffer, 25, offset, itemId, value1, value2);

	// Check the float values
	uint8_t expectedValue1[4];
	uint8_t expectedValue2[4];
	floatToBytes(value1, expectedValue1);
	floatToBytes(value2, expectedValue2);

	assert_int_equal(buffer[offset], 0x1F);
	assert_int_equal(buffer[offset + 1], itemId);
	assert_memory_equal(&buffer[offset + 2], expectedValue1, DASHBOARD_SIZE_OF_FLOAT);
	assert_memory_equal(&buffer[offset + 2 + DASHBOARD_SIZE_OF_FLOAT], expectedValue2, DASHBOARD_SIZE_OF_FLOAT);
}

static void test_decodeOldProto(void **state) {
	(void) state; // unused

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;

    buffer[1] = 'H';
    buffer[2] = 'e';
    buffer[3] = 'y';
    buffer[4] = '!';

	char labelBuffer[18];

	uint8_t decodedId = decodeToItemLabel(buffer, 25, offset, labelBuffer, 18);

	assert_int_equal(decodedId, UNKNOWN_ITEM_ID);

	assert_string_equal(labelBuffer, "Hey!");

}

static void test_decodeToItemLabelNoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToDashboardMessage(buffer, 25, offset, FIRMWARE_ITEM.id, 12.34f, -1);

	char labelBuffer[18];

	uint8_t decodedId = decodeToItemLabel(buffer, 25, offset, labelBuffer, 18);
	assert_int_equal(decodedId, FIRMWARE_ITEM.id);
	snprintf_(labelBuffer, 18, "%s", labelBuffer);//getting the null terminator just for the testing, string will be 1 char shorter
	assert_string_equal(labelBuffer, "BACCAble " BUILD_VERSION "    ");

}

static void test_decodeToItemLabelOneValue(void **state) {
	(void) state; // unused

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToDashboardMessage(buffer, 25, offset, HP_ITEM.id, 12.34f, -1);

	char labelBuffer[18];

	uint8_t decodedId = decodeToItemLabel(buffer, 25, offset, labelBuffer, 18);
	assert_int_equal(decodedId, HP_ITEM.id);

	snprintf_(labelBuffer, 18, "%s", labelBuffer);//getting the null terminator just for the testing, string will be 1 char shorter
	assert_string_equal(labelBuffer, "Power: 12.3hp    ");

}

static void test_decodeToItemLabelTwoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToDashboardMessage(buffer, 25, offset, FRONT_TIRES_TEMP_ITEM.id, 12.34f, 56.78f);

	char labelBuffer[18];

	uint8_t decodedId = decodeToItemLabel(buffer, 25, offset, labelBuffer, 18);
	assert_int_equal(decodedId, FRONT_TIRES_TEMP_ITEM.id);

	snprintf_(labelBuffer, 18, "%s", labelBuffer);//getting the null terminator just for the testing, string will be 1 char shorter
	assert_string_equal(labelBuffer, "12""\xB0""C F.T. 57""\xB0""C   ");// 2nd value is rounded

}

static void test_decodeAllItems(void **state) {
	(void) state; // unused

	uint8_t items[] = {
		    CLEANUP_ITEM_ID,
		    FIRMWARE_ITEM_ID,
		    HP_ITEM_ID,
		    TORQUE_ITEM_ID,
		    DPF_CLOG_ITEM_ID,
		    DPF_TEMP_ITEM_ID,
		    DPF_REG_ITEM_ID,
		    DPF_DIST_ITEM_ID,
		    DPF_COUNT_ITEM_ID,
		    DPF_MEAN_DIST_ITEM_ID,
			DPF_MEAN_DURATION_ITEM_ID,
		    BATTERY_V_ITEM_ID,
		    BATTERY_P_ITEM_ID,
		    BATTERY_A_ITEM_ID,
		    OIL_QUALITY_ITEM_ID,
		    OIL_TEMP_ITEM_ID,
		    OIL_PRESS_ITEM_ID,
		    AIR_IN_ITEM_ID,
		    GEAR_ITEM_ID,
		    GEARBOX_TEMP_ITEM_ID,
			FRONT_TIRES_TEMP_ITEM_ID,
			REAR_TIRES_TEMP_ITEM_ID,
		};

	uint8_t len = sizeof(items) / sizeof(uint8_t);

	char *labels[] = {
			"                  ",
			"BACCAble " BUILD_VERSION "     ",
			"Power: 1.2hp      ",
			"Torque: 1nm       ",
			"DPF clog: 1%      ",
			"DPF temp: 1""\xB0""C     ",
			"DPF reg: 1%       ",
			"DPF dist: 1km     ",
			"DPF count: 1      ",
			"DPF mean: 1km     ",
			"DPF mean: 1min    ",
			"Battery: 1.23V    ",
			"Battery: 1%       ",
			"Battery: 1.23A    ",
			"Oil qlt: 1%       ",
			"Oil temp: 1""\xB0""C     ",
			"Oil press: 1.2bar ",
			"Air in temp: 1""\xB0""C  ",
			"Current gear: 1   ",
			"Gearbox: 1""\xB0""C      ",
			"1""\xB0""C F.T. 57""\xB0""C     ",
			"1""\xB0""C R.T. 57""\xB0""C     ",
		};

	uint8_t buffer[25] = {0};
	uint8_t offset = 1;

	char labelBuffer[19];

	for (int i = 0; i<len; i++) {

		memset(labelBuffer, ' ', sizeof(labelBuffer));
		uint8_t itemId = items[i];
		char *label = labels[i];
		encodeToDashboardMessage(buffer, 25, offset, itemId, 1.234f, 56.78f);
		uint8_t decodedId = decodeToItemLabel(buffer, 25, offset, labelBuffer, 18);
		snprintf_(labelBuffer, 19, "%s", labelBuffer);//getting the null terminator just for the testing, string will be 1 char shorter
		assert_int_equal(decodedId, itemId);
		assert_string_equal(labelBuffer, label);
	}

	// Call the function under test

}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_encodeToDashboardMessage),
	        cmocka_unit_test(test_decodeOldProto),
	        cmocka_unit_test(test_decodeToItemLabelNoValues),
	        cmocka_unit_test(test_decodeToItemLabelOneValue),
	        cmocka_unit_test(test_decodeToItemLabelTwoValues),
	        cmocka_unit_test(test_decodeAllItems),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
