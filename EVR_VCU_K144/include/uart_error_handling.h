#ifndef UART_ERROR_HANDLING_H
#define UART_ERROR_HANDLING_H

#ifdef __cplusplus
extern "C"{
#endif

#include "Mcu.h"


#define ERROR 9
#define TEMP_SENSOR 10
#define BMS_VOLTAGE 11
#define BMS_CURRENT 12
#define ACCELERATOR_PEDALS 13
#define BRAKE_PEDAL 14
#define SEVEN_SEGMENT 15
#define PROCESSOR 16

#define SEVEN_SEG_NO_RESPONSE 0
#define SEVEN_SEG_NUMBER_TOO_LARGE 1
#define ACCELERATOR_PEDALS_DIFFERENT_OUTPUT 0
#define TEMPERATURE_TOO_HIGH 0
#define BMS_NO_RESPONSE 0
#define BMS_LOW_VOLTAGE 1
#define BMS_HIGH_CONSUMPTION 2
#define PROCESSOR_RESET 0

typedef struct{
	uint8 seven_seg_error;
	uint8 accelerator_pedals_error;
	uint8 temperature_error;
	uint8 bms_error;
	uint8 processor_error;
}Errors;

void ErrorsSet(uint8 Module, uint8 Error);
uint8 ErrorsGet(uint8 Module);
void ErrorsClear(uint8 Module, uint8 Error);

#ifdef __cplusplus
}
#endif

#endif
