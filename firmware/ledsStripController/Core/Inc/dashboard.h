/*
 * dashboard.h
 *
 */

 #ifndef INC_DASHBOARD_H_
 #define INC_DASHBOARD_H_
 
 #include "compile_time_defines.h"
 #include "stdint.h"
 #include "string.h"
 #include "printf.h"
 
 static const int DASHBOARD_SIZE_OF_FLOAT = sizeof(float);
 static const float NAN_FLOAT = 0.0f / 0.0f;
 
 void encodeToDashboardMessage(uint8_t* buffer, uint8_t bufferSize, uint8_t bufferOffset, uint8_t itemId, float firstValue, float secondValue);
 
 #define UNKNOWN_ITEM_ID 255
 #define CLEANUP_ITEM_ID 0
 #define FIRMWARE_ITEM_ID 1
 #define HP_ITEM_ID 2
 #define TORQUE_ITEM_ID 3
 #define DPF_CLOG_ITEM_ID 4
 #define DPF_TEMP_ITEM_ID 5
 #define DPF_REG_ITEM_ID 6
 #define DPF_DIST_ITEM_ID 7
 #define DPF_COUNT_ITEM_ID 8
 #define DPF_AVG_DIST_ITEM_ID 9
 #define DPF_AVG_DURATION_ITEM_ID 10
 #define BATTERY_V_ITEM_ID 11
 #define BATTERY_P_ITEM_ID 12
 #define BATTERY_A_ITEM_ID 13
 #define OIL_QUALITY_ITEM_ID 14
 #define OIL_TEMP_ITEM_ID 15
 #define OIL_PRESS_ITEM_ID 16
 #define AIR_IN_ITEM_ID 17
 #define GEAR_ITEM_ID 18
 #define GEARBOX_TEMP_ITEM_ID 19
 #define FRONT_TIRES_TEMP_ITEM_ID 20
 #define REAR_TIRES_TEMP_ITEM_ID 21
 
 #ifdef BHbaccable
 typedef struct {
     uint8_t		id;
     char		*pattern;
 } DashboardItem;
 
 uint8_t decodeToItemLabel(uint8_t *buffer, uint8_t bufferSize, uint8_t bufferOffset, char *labelBuffer, uint8_t labelBufferMaxLength);
 
 static const DashboardItem CLEANUP_ITEM = { .id = CLEANUP_ITEM_ID };
 static const DashboardItem FIRMWARE_ITEM = { .id = FIRMWARE_ITEM_ID, .pattern = "BACCAble " BUILD_VERSION };
 static const DashboardItem HP_ITEM = { .id = HP_ITEM_ID, .pattern = "Power: %.1fhp" };
 static const DashboardItem TORQUE_ITEM = { .id = TORQUE_ITEM_ID, .pattern = "Torque: %.0fnm" };
 static const DashboardItem DPF_CLOG_ITEM = { .id = DPF_CLOG_ITEM_ID, .pattern = "DPF clog: %.0f%%" };
 static const DashboardItem DPF_TEMP_ITEM = { .id = DPF_TEMP_ITEM_ID, .pattern = "DPF temp: %.0f°C" };
 static const DashboardItem DPF_REG_ITEM = { .id = DPF_REG_ITEM_ID, .pattern = "DPF reg: %.0f%%" };
 static const DashboardItem DPF_DIST_ITEM = { .id = DPF_DIST_ITEM_ID, .pattern = "DPF dist: %.0fkm" };
 static const DashboardItem DPF_COUNT_ITEM = { .id = DPF_COUNT_ITEM_ID, .pattern = "DPF count: %.0f" };
 static const DashboardItem DPF_AVG_DIST_ITEM = { .id = DPF_AVG_DIST_ITEM_ID, .pattern = "DPF avg: %.0fkm" };
 static const DashboardItem DPF_AVG_DURATION_ITEM = { .id = DPF_AVG_DURATION_ITEM_ID, .pattern = "DPF avg: %.0fmin" };
 static const DashboardItem BATTERY_V_ITEM = { .id = BATTERY_V_ITEM_ID, .pattern = "Battery: %.2fV" };
 static const DashboardItem BATTERY_P_ITEM = { .id = BATTERY_P_ITEM_ID, .pattern = "Battery: %.0f%%" };
 static const DashboardItem BATTERY_A_ITEM = { .id = BATTERY_A_ITEM_ID, .pattern = "Battery: %.2fA" };
 static const DashboardItem OIL_QUALITY_ITEM = { .id = OIL_QUALITY_ITEM_ID, .pattern = "Oil deg: %.0f%%" };
 static const DashboardItem OIL_TEMP_ITEM = { .id = OIL_TEMP_ITEM_ID, .pattern = "Oil temp: %.0f°C" };
 static const DashboardItem OIL_PRESS_ITEM = { .id = OIL_PRESS_ITEM_ID, .pattern = "Oil press: %.1fbar" };
 static const DashboardItem AIR_IN_ITEM = { .id = AIR_IN_ITEM_ID, .pattern = "Air in temp: %.0f°C" };
 static const DashboardItem GEAR_ITEM = { .id = GEAR_ITEM_ID, .pattern = "Current gear: %c" };//will have dedicated decoder
 static const DashboardItem GEARBOX_TEMP_ITEM = { .id = GEARBOX_TEMP_ITEM_ID, .pattern = "Gearbox: %.0f°C" };
 static const DashboardItem FRONT_TIRES_TEMP_ITEM = { .id = FRONT_TIRES_TEMP_ITEM_ID, .pattern = "%.0f°C F.T. %.0f°C" };
 static const DashboardItem REAR_TIRES_TEMP_ITEM = { .id = REAR_TIRES_TEMP_ITEM_ID, .pattern = "%.0f°C R.T. %.0f°C" };
 #endif

 #endif /* INC_DASHBOARD_H_ */