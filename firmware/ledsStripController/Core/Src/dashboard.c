/*
 * dashboard.c
 *
 */

 #include "dashboard.h"
 #include "stdbool.h"

 const uint8_t PROTO_MARKER = 0x1F;

void encodeToMessage(uint8_t *buffer, uint8_t bufferSize, uint8_t bufferOffset, uint8_t itemId,
         float firstValue, float secondValue) {

    uint8_t protoIndex = bufferOffset;
    if (bufferSize > protoIndex) {
        buffer[protoIndex] = PROTO_MARKER;
    }

    uint8_t itemIdIndex = protoIndex + 1;
     if (bufferSize > itemIdIndex) {
         buffer[itemIdIndex] = itemId;
     }
 
     uint8_t firstValueIndex = itemIdIndex + 1;
     if (bufferSize > firstValueIndex) {
         memcpy(&buffer[firstValueIndex], &firstValue, DASHBOARD_SIZE_OF_FLOAT);
     }
 
     uint8_t secondValueIndex = firstValueIndex + DASHBOARD_SIZE_OF_FLOAT;
     if (bufferSize > secondValueIndex) {
         memcpy(&buffer[secondValueIndex], &secondValue,
                 DASHBOARD_SIZE_OF_FLOAT);
     }
}

#ifdef BHbaccable
const uint8_t GEARS[]={'N','1','2','3','4','5','6','R','7','8','9'};

bool decodeFromMessage(uint8_t *buffer, uint8_t bufferSize, uint8_t bufferOffset, uint8_t *itemId,
         float *firstValue, float *secondValue) {
 
    uint8_t protoIndex = bufferOffset;
    if (bufferSize <= protoIndex || buffer[protoIndex] != PROTO_MARKER) {
        *itemId = 0;
        *firstValue = NAN_FLOAT;
        *secondValue = NAN_FLOAT;
        return false;
    }

    uint8_t itemIdIndex = protoIndex + 1;
     if (bufferSize > itemIdIndex) {
         *itemId = buffer[itemIdIndex];  // Decode itemId
     } else {
         *itemId = 0;
     }
 
     uint8_t firstValueIndex = itemIdIndex + 1;
     if (bufferSize > firstValueIndex) {
         memcpy(firstValue, &buffer[firstValueIndex], DASHBOARD_SIZE_OF_FLOAT); // Decode firstValue
     } else {
         *firstValue = NAN_FLOAT;
     }
 
     uint8_t secondValueIndex = firstValueIndex + DASHBOARD_SIZE_OF_FLOAT;
     if (bufferSize > secondValueIndex) {
         memcpy(secondValue, &buffer[secondValueIndex],
                 DASHBOARD_SIZE_OF_FLOAT);  // Decode secondValue
     } else {
         *secondValue = NAN_FLOAT;
     }
     
     return true;
 }
 
 const char* itemLabelFormat(uint8_t itemId) {
     if (itemId == FIRMWARE_ITEM_ID)
         return FIRMWARE_ITEM.pattern;
     else if (itemId == HP_ITEM_ID)
         return HP_ITEM.pattern;
     else if (itemId == TORQUE_ITEM_ID)
         return TORQUE_ITEM.pattern;
     else if (itemId == DPF_CLOG_ITEM_ID)
         return DPF_CLOG_ITEM.pattern;
     else if (itemId == DPF_TEMP_ITEM_ID)
         return DPF_TEMP_ITEM.pattern;
     else if (itemId == DPF_REG_ITEM_ID)
         return DPF_REG_ITEM.pattern;
     else if (itemId == DPF_DIST_ITEM_ID)
         return DPF_DIST_ITEM.pattern;
     else if (itemId == DPF_COUNT_ITEM_ID)
         return DPF_COUNT_ITEM.pattern;
     else if (itemId == DPF_AVG_DIST_ITEM_ID)
         return DPF_AVG_DIST_ITEM.pattern;
     else if (itemId == DPF_AVG_DURATION_ITEM_ID)
         return DPF_AVG_DURATION_ITEM.pattern;
     else if (itemId == BATTERY_V_ITEM_ID)
         return BATTERY_V_ITEM.pattern;
     else if (itemId == BATTERY_P_ITEM_ID)
         return BATTERY_P_ITEM.pattern;
     else if (itemId == BATTERY_A_ITEM_ID)
         return BATTERY_A_ITEM.pattern;
     else if (itemId == OIL_QUALITY_ITEM_ID)
         return OIL_QUALITY_ITEM.pattern;
     else if (itemId == OIL_TEMP_ITEM_ID)
         return OIL_TEMP_ITEM.pattern;
     else if (itemId == OIL_PRESS_ITEM_ID)
         return OIL_PRESS_ITEM.pattern;
     else if (itemId == AIR_IN_ITEM_ID)
         return AIR_IN_ITEM.pattern;
     else if (itemId == GEAR_ITEM_ID)
         return GEAR_ITEM.pattern;
     else if (itemId == GEARBOX_TEMP_ITEM_ID)
         return GEARBOX_TEMP_ITEM.pattern;
     else if (itemId == FRONT_TIRES_TEMP_ITEM_ID)
         return FRONT_TIRES_TEMP_ITEM.pattern;
     else if (itemId == REAR_TIRES_TEMP_ITEM_ID)
         return REAR_TIRES_TEMP_ITEM.pattern;
     else
         return "";
 }
 
 char floatToGear(float num) {
    uint8_t i = (uint8_t)num;
    if (i >= sizeof(GEARS) || i < 0) {
        return '-';
    }
    return GEARS[i];
 }
 
 uint8_t decodeToItemLabel(uint8_t *buffer, uint8_t bufferSize, uint8_t bufferOffset, char *labelBuffer, uint8_t labelBufferMaxLength) {
 
     uint8_t decodedItemId;
     float decodedFirstValue;
     float decodedSecondValue;
 
     bool decoded = decodeFromMessage(buffer, bufferSize, bufferOffset, &decodedItemId, &decodedFirstValue,
             &decodedSecondValue);
 
    if(decoded) {
        const char *format = itemLabelFormat(decodedItemId);

        if (decodedItemId == GEAR_ITEM_ID) {
            snprintf_(labelBuffer, labelBufferMaxLength, format, floatToGear(decodedFirstValue));
        } else {
            snprintf_(labelBuffer, labelBufferMaxLength, format, decodedFirstValue, decodedSecondValue);
        }

        return decodedItemId;
    } else {
        snprintf_(labelBuffer, labelBufferMaxLength, "%s", buffer + 1);
        return 0;
    }
}
#endif