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

	assert_memory_equal(&buffer[offset + 1], expectedValue1, DASHBOARD_SIZE_OF_FLOAT);
	assert_memory_equal(&buffer[offset + 1 + DASHBOARD_SIZE_OF_FLOAT], expectedValue2, DASHBOARD_SIZE_OF_FLOAT);
}

static void test_decodeToItemLabelNoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, FIRMWARE_ITEM.id, 12.34f, -1);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, labelBuffer, 18);

	assert_string_equal(labelBuffer, "BACCAble 3.0");

}

static void test_decodeToItemLabelOneValue(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, HP_ITEM.id, 12.34f, -1);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, labelBuffer, 18);

	assert_string_equal(labelBuffer, "Power: 12.3hp");

}

static void test_decodeToItemLabelTwoValues(void **state) {
	(void) state; // unused

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	// Call the function under test
	encodeToMessage(buffer, offset, FRONT_TIRES_TEMP_ITEM.id, 12.34f, 56.78f);

	char labelBuffer[18];

	decodeToItemLabel(buffer, offset, labelBuffer, 18);

	assert_string_equal(labelBuffer, "12°C F.T. 57°C");// 2nd value is rounded

}

static void test_decodeAllItems(void **state) {
	(void) state; // unused

	DashboardItem items[] = {
		    CLEANUP_ITEM,
		    FIRMWARE_ITEM,
		    HP_ITEM,
		    TORQUE_ITEM,
		    DPF_CLOG_ITEM,
		    DPF_TEMP_ITEM,
		    DPF_REG_ITEM,
		    DPF_DIST_ITEM,
		    DPF_COUNT_ITEM,
		    DPF_AVG_DIST_ITEM,
		    DPF_AVG_TIME_ITEM,
		    BATTERY_V_ITEM,
		    BATTERY_P_ITEM,
		    BATTERY_A_ITEM,
		    OIL_QUALITY_ITEM,
		    OIL_TEMP_ITEM,
		    OIL_PRESS_ITEM,
		    AIR_IN_ITEM,
		    GEAR_ITEM,
		    GEARBOX_TEMP_ITEM,
			FRONT_TIRES_TEMP_ITEM,
			REAR_TIRES_TEMP_ITEM,
		};

	uint8_t len = sizeof(items) / sizeof(DashboardItem);

	char *labels[] = {
			"",
			"BACCAble 3.0",
			"Power: 12.3hp",
			"Torque: 12nm",
			"DPF clog: 12%",
			"DPF temp: 12°C",
			"DPF reg: 12%",
			"DPF dist: 12km",
			"DPF count: 12",
			"DPF avg: 12km",
			"DPF avg: 12min",
			"Battery: 12.34V",
			"Battery: 12%",
			"Battery: 12.34A",
			"Oil deg: 12%",
			"Oil temp: 12°C",
			"Oil press: 12.3bar",
			"Air in temp: 12°C",
			"Current gear: -",
			"Gearbox: 12°C",
			"12°C F.T. 57°C",
			"12°C R.T. 57°C",
		};

	uint8_t buffer[20] = {0};
	uint8_t offset = 1;

	char labelBuffer[19];

	for (int i = 0; i<len; i++) {

		memset(labelBuffer, '\0', sizeof(labelBuffer));

		DashboardItem item = items[i];
		char *label = labels[i];
		encodeToMessage(buffer, offset, item.id, 12.34f, 56.78f);
		decodeToItemLabel(buffer, offset, labelBuffer, 19);
		assert_string_equal(labelBuffer, label);
	}

	// Call the function under test

}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_encodeToMessage),
	        cmocka_unit_test(test_decodeToItemLabelNoValues),
	        cmocka_unit_test(test_decodeToItemLabelOneValue),
	        cmocka_unit_test(test_decodeToItemLabelTwoValues),
	        cmocka_unit_test(test_decodeAllItems),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
