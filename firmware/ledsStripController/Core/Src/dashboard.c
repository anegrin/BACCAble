/*
 * dashboard.c
 *
 */

#include "dashboard.h"

void encodeToMessage(uint8_t *buffer, uint8_t bufferOffset, uint8_t itemId,
		float firstValue, float secondValue) {
	uint8_t itemIdIndex = bufferOffset;
	if (sizeof(buffer) > itemIdIndex) {
		buffer[itemIdIndex] = itemId;
	}

	uint8_t firstValueIndex = bufferOffset + 1;
	if (sizeof(buffer) > firstValueIndex) {
		memcpy(&buffer[firstValueIndex], &firstValue, DASHBOARD_SIZE_OF_FLOAT);
	}

	uint8_t secondValueIndex = firstValueIndex + DASHBOARD_SIZE_OF_FLOAT;
	if (sizeof(buffer) > secondValueIndex) {
		memcpy(&buffer[secondValueIndex], &secondValue,
				DASHBOARD_SIZE_OF_FLOAT);
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
		memcpy(firstValue, &buffer[firstValueIndex], DASHBOARD_SIZE_OF_FLOAT); // Decode firstValue
	} else {
		*firstValue = NAN_FLOAT;
	}

	uint8_t secondValueIndex = firstValueIndex + DASHBOARD_SIZE_OF_FLOAT;
	if (sizeof(buffer) > secondValueIndex) {
		memcpy(secondValue, &buffer[secondValueIndex],
				DASHBOARD_SIZE_OF_FLOAT);  // Decode secondValue
	} else {
		*secondValue = NAN_FLOAT;
	}
}

const char* itemLabelFormat(uint8_t itemId) {
	if (itemId == FIRMWARE_ITEM.id)
		return FIRMWARE_ITEM.pattern;
	else if (itemId == HP_ITEM.id)
		return HP_ITEM.pattern;
	else if (itemId == TORQUE_ITEM.id)
		return TORQUE_ITEM.pattern;
	else if (itemId == DPF_CLOG_ITEM.id)
		return DPF_CLOG_ITEM.pattern;
	else if (itemId == DPF_TEMP_ITEM.id)
		return DPF_TEMP_ITEM.pattern;
	else if (itemId == DPF_REG_ITEM.id)
		return DPF_REG_ITEM.pattern;
	else if (itemId == DPF_DIST_ITEM.id)
		return DPF_DIST_ITEM.pattern;
	else if (itemId == DPF_COUNT_ITEM.id)
		return DPF_COUNT_ITEM.pattern;
	else if (itemId == DPF_AVG_DIST_ITEM.id)
		return DPF_AVG_DIST_ITEM.pattern;
	else if (itemId == DPF_AVG_DURATION_ITEM.id)
		return DPF_AVG_DURATION_ITEM.pattern;
	else if (itemId == BATTERY_V_ITEM.id)
		return BATTERY_V_ITEM.pattern;
	else if (itemId == BATTERY_P_ITEM.id)
		return BATTERY_P_ITEM.pattern;
	else if (itemId == BATTERY_A_ITEM.id)
		return BATTERY_A_ITEM.pattern;
	else if (itemId == OIL_QUALITY_ITEM.id)
		return OIL_QUALITY_ITEM.pattern;
	else if (itemId == OIL_TEMP_ITEM.id)
		return OIL_TEMP_ITEM.pattern;
	else if (itemId == OIL_PRESS_ITEM.id)
		return OIL_PRESS_ITEM.pattern;
	else if (itemId == AIR_IN_ITEM.id)
		return AIR_IN_ITEM.pattern;
	else if (itemId == GEAR_ITEM.id)
		return GEAR_ITEM.pattern;
	else if (itemId == GEARBOX_TEMP_ITEM.id)
		return GEARBOX_TEMP_ITEM.pattern;
	else if (itemId == FRONT_TIRES_TEMP_ITEM.id)
		return FRONT_TIRES_TEMP_ITEM.pattern;
	else if (itemId == REAR_TIRES_TEMP_ITEM.id)
		return REAR_TIRES_TEMP_ITEM.pattern;
	else
		return "";
}

char floatToGear(float num) {
	switch ((int) num) {
	case 0:
		return 'N';
	case 10: // seems like 10 can also be 'R'
	case 16:
		return 'R';
	default:
		if (num > 0 && num < 9)
			return 48 + num;
		else {
			return '-';
		}
	}
}

uint8_t decodeToItemLabel(uint8_t *buffer, uint8_t bufferOffset, char *labelBuffer, uint8_t labelBufferMaxLength) {

	uint8_t decodedItemId;
	float decodedFirstValue;
	float decodedSecondValue;

	decodeFromMessage(buffer, bufferOffset, &decodedItemId, &decodedFirstValue,
			&decodedSecondValue);

	const char *format = itemLabelFormat(decodedItemId);

	if (decodedItemId == GEAR_ITEM.id) {
		snprintf(labelBuffer, labelBufferMaxLength, format, floatToGear(decodedFirstValue));
	} else {
		snprintf(labelBuffer, labelBufferMaxLength, format, decodedFirstValue, decodedSecondValue);
	}

	return decodedItemId;
}
