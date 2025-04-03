/*
 * dashboard.h
 *
 */

#ifndef INC_DASHBOARD_H_
#define INC_DASHBOARD_H_

#include "stdint.h"
#include "string.h"
#include "printf.h"

typedef struct {
	uint8_t		id;
	char		*pattern;
} DashboardItem;

void encodeToMessage(uint8_t* buffer, uint8_t bufferOffset, uint8_t itemId, float firstValue, float secondValue);
void decodeToItemLabel(uint8_t* buffer, uint8_t bufferOffset, uint8_t itemId, char* labelBuffer, uint8_t labelBufferMaxLength);

#endif /* INC_DASHBOARD_H_ */
