#ifndef UART_ERROR_HANDLING_H
#define UART_ERROR_HANDLING_H

#ifdef __cplusplus
extern "C"{
#endif

#include "Mcu.h"
#include "uart_datasend.h"

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
