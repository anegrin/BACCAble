/*
 * dashboard.c
 *
 */

#include "dashboard.h"

const int _DASHBOARD_SIZE_OF_FLOAT = sizeof(float);

const float NAN_FLOAT = 0.0f / 0.0f;

const DashboardItem FIRMWARE_ITEM = { .id = 0, .pattern = "BACCAble 3.0" };
const DashboardItem HP_ITEM = { .id = 1, .pattern = "Power: %.0fhp" };
const DashboardItem FRONT_TIRES_TEMP_ITEM = { .id = 2, .pattern = "%.0f°C FT %.0f°C" };

void encodeToMessage(uint8_t *buffer, uint8_t bufferOffset, uint8_t itemId,
		float firstValue, float secondValue) {
	uint8_t itemIdIndex = bufferOffset;
	if (sizeof(buffer) > itemIdIndex) {
		buffer[itemIdIndex] = itemId;
	}

	uint8_t firstValueIndex = bufferOffset + 1;
	if (sizeof(buffer) > firstValueIndex) {
		memcpy(&buffer[firstValueIndex], &firstValue, _DASHBOARD_SIZE_OF_FLOAT);
	}

	uint8_t secondValueIndex = firstValueIndex + _DASHBOARD_SIZE_OF_FLOAT;
	if (sizeof(buffer) > secondValueIndex) {
		memcpy(&buffer[secondValueIndex], &secondValue,
				_DASHBOARD_SIZE_OF_FLOAT);
	}
}

void decodeFromMessage(uint8_t *buffer, uint8_t bufferOffset, uint8_t *itemId,
		float *firstValue, float *secondValue) {

	uint8_t itemIdIndex = bufferOffset;
	if (sizeof(buffer) > itemIdIndex) {
		*itemId = buffer[itemIdIndex];  // Decode itemId
	} else {
		*itemId = 0;
	}

	uint8_t firstValueIndex = bufferOffset + 1;
	if (sizeof(buffer) > firstValueIndex) {
		memcpy(firstValue, &buffer[firstValueIndex], _DASHBOARD_SIZE_OF_FLOAT); // Decode firstValue
	} else {
		*firstValue = NAN_FLOAT;
	}

	uint8_t secondValueIndex = firstValueIndex + _DASHBOARD_SIZE_OF_FLOAT;
	if (sizeof(buffer) > secondValueIndex) {
		memcpy(secondValue, &buffer[secondValueIndex],
				_DASHBOARD_SIZE_OF_FLOAT);  // Decode secondValue
	} else {
		*secondValue = NAN_FLOAT;
	}
}

const char* itemLabelFormat(uint8_t itemId) {
	if (itemId == FIRMWARE_ITEM.id) {
		return FIRMWARE_ITEM.pattern;
	} else if (itemId == HP_ITEM.id) {
		return HP_ITEM.pattern;
	} else if (itemId == FRONT_TIRES_TEMP_ITEM.id) {
		return FRONT_TIRES_TEMP_ITEM.pattern;
	} else {
		return "";
	}
}

void decodeToItemLabel(uint8_t *buffer, uint8_t bufferOffset, uint8_t itemId,
		char *labelBuffer, uint8_t labelBufferMaxLength) {

	uint8_t decodedItemId;
	float decodedFirstValue;
	float decodedSecondValue;

	decodeFromMessage(buffer, bufferOffset, &decodedItemId, &decodedFirstValue,
			&decodedSecondValue);

	const char *format = itemLabelFormat(itemId);

	snprintf(labelBuffer, labelBufferMaxLength, format,
			decodedFirstValue, decodedSecondValue);
}
