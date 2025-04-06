/*
 * dashboard.h
 *
 */

#ifndef INC_DASHBOARD_H_
#define INC_DASHBOARD_H_

#include "stdint.h"
#include "string.h"
#include "printf.h"

static const int DASHBOARD_SIZE_OF_FLOAT = sizeof(float);
static const float NAN_FLOAT = 0.0f / 0.0f;


typedef struct {
	uint8_t		id;
	char		*pattern;
} DashboardItem;

void encodeToMessage(uint8_t* buffer, uint8_t bufferOffset, uint8_t itemId, float firstValue, float secondValue);
void decodeToItemLabel(uint8_t* buffer, uint8_t bufferOffset, char* labelBuffer, uint8_t labelBufferMaxLength);

static const DashboardItem CLEANUP_ITEM = { .id = 0 };
static const DashboardItem FIRMWARE_ITEM = { .id = 1, .pattern = "BACCAble 3.0" };
static const DashboardItem HP_ITEM = { .id = 2, .pattern = "Power: %.1fhp" };
static const DashboardItem TORQUE_ITEM = { .id = 3, .pattern = "Torque: %.0fnm" };
static const DashboardItem DPF_CLOG_ITEM = { .id = 4, .pattern = "DPF clog: %.0f%%" };
static const DashboardItem DPF_TEMP_ITEM = { .id = 5, .pattern = "DPF temp: %.0f°C" };
static const DashboardItem DPF_REG_ITEM = { .id = 6, .pattern = "DPF reg: %.0f%%" };
static const DashboardItem DPF_DIST_ITEM = { .id = 7, .pattern = "DPF dist: %.0fkm" };
static const DashboardItem DPF_COUNT_ITEM = { .id = 8, .pattern = "DPF count: %.0f" };
static const DashboardItem DPF_AVG_DIST_ITEM = { .id = 9, .pattern = "DPF avg: %.0fkm" };
static const DashboardItem DPF_AVG_TIME_ITEM = { .id = 10, .pattern = "DPF avg: %.0fmin" };
static const DashboardItem BATTERY_V_ITEM = { .id = 11, .pattern = "Battery: %.2fV" };
static const DashboardItem BATTERY_P_ITEM = { .id = 12, .pattern = "Battery: %.0f%%" };
static const DashboardItem BATTERY_A_ITEM = { .id = 13, .pattern = "Battery: %.2fA" };
static const DashboardItem OIL_QUALITY_ITEM = { .id = 14, .pattern = "Oil deg: %.0f%%" };
static const DashboardItem OIL_TEMP_ITEM = { .id = 15, .pattern = "Oil temp: %.0f°C" };
static const DashboardItem OIL_PRESS_ITEM = { .id = 16, .pattern = "Oil press: %.1fbar" };
static const DashboardItem AIR_IN_ITEM = { .id = 17, .pattern = "Air in temp: %.0f°C" };
static const DashboardItem GEAR_ITEM = { .id = 18, .pattern = "Current gear: %c" };//will have dedicated decoder
static const DashboardItem GEARBOX_TEMP_ITEM = { .id = 19, .pattern = "Gearbox: %.0f°C" };
static const DashboardItem FRONT_TIRES_TEMP_ITEM = { .id = 20, .pattern = "%.0f°C F.T. %.0f°C" };
static const DashboardItem REAR_TIRES_TEMP_ITEM = { .id = 21, .pattern = "%.0f°C R.T. %.0f°C" };

#endif /* INC_DASHBOARD_H_ */
