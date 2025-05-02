#include "uart_proto.h"

void fill_buffer(uint8_t *buffer, uint8_t bufferLength, const uint8_t *data, uint8_t dataLength) {
    memset(buffer, 0xFF, bufferLength - 1);
    if (dataLength>bufferLength - 1) dataLength = bufferLength - 1;
    memcpy(buffer, data, dataLength);
    buffer[bufferLength - 1] = calculate_crc8(buffer, bufferLength - 1);
}

/* impl takes around 200 ns on STM32H7 */
uint8_t calculate_crc8(const uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < length; i++) {
        crc = CRC8_TABLE[crc ^ data[i]];
    }
    return crc;
}

bool isCRCValid(uint8_t *buffer, uint8_t bufferSize) {
    return buffer[bufferSize - 1] == calculate_crc8(buffer, bufferSize - 1);
}

bool validateRxBuffer(uint8_t *buffer, uint8_t bufferLength){
	uint8_t requestId = buffer[0];

	switch(requestId) {
		case C1BusID:
			return (
				buffer[1] == C2cmdNormalFrontBrake || 
				buffer[1] == C2cmdForceFrontBrake	
			) && isCRCValid(buffer, bufferLength);
		case C2BusID:

			return (
				buffer[1] == C2cmdtoggleDyno || 
				buffer[1] == C2cmdNormalFrontBrake || 
				buffer[1] == C2cmdForceFrontBrake || 
				buffer[1] == C2cmdGetStatus || 
				buffer[1] == C2cmdtoggleEscTc
			) && isCRCValid(buffer, bufferLength);
		case AllSleep:
		case C2BusIDAllSleepAck:
		case BHBusIDAllSleepAck:
		case AllResetFaults:
		case BhBusIDgetStatus:
		case BhBusChimeRequest:
			return isCRCValid(buffer, bufferLength);
		case BhBusIDparamString:
			return isCRCValid(buffer, bufferLength);
		default:
			return false;
	}
}
