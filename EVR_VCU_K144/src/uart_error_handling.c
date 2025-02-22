#ifdef __cplusplus
extern "C" {
#endif

#include "uart_error_handling.h"
#include "CDD_Uart.h"




Errors errors_instance;

void ErrorsSet(uint8 Module, uint8 Error)
{
	switch (Module) {
	case TEMP_SENSOR:
		errors_instance.temperature_error = errors_instance.temperature_error | (1 << Error);
	break;
	case BMS_VOLTAGE:
	case BMS_CURRENT:
		errors_instance.bms_error = errors_instance.bms_error | (1 << Error);
	break;
	case ACCELERATOR_PEDALS:
		errors_instance.accelerator_pedals_error = errors_instance.accelerator_pedals_error | (1 << Error);
	break;
	case SEVEN_SEGMENT:
		errors_instance.seven_seg_error = errors_instance.seven_seg_error | (1 << Error);
	break;
	case PROCESSOR:
		errors_instance.processor_error = errors_instance.processor_error | (1 << Error);
	break;
	default:
		;
	}
}
uint8 ErrorsGet(uint8 Module)
{
	switch (Module) {
		case TEMP_SENSOR:
			return errors_instance.temperature_error;
		break;
		case BMS_VOLTAGE:
		case BMS_CURRENT:
			return errors_instance.bms_error;
		break;
		case ACCELERATOR_PEDALS:
			return errors_instance.accelerator_pedals_error;
		break;
		case SEVEN_SEGMENT:
			return errors_instance.seven_seg_error;
		break;
		case PROCESSOR:
			return errors_instance.processor_error;
		break;
		default:
			return 0;
		}
}
void ErrorsClear(uint8 Module, uint8 Error)
{
	switch (Module) {
		case TEMP_SENSOR:
			errors_instance.temperature_error = errors_instance.temperature_error & (~(1 << Error));
		break;
		case BMS_VOLTAGE:
		case BMS_CURRENT:
			errors_instance.bms_error = errors_instance.bms_error & (~(1 << Error));
		break;
		case ACCELERATOR_PEDALS:
			errors_instance.accelerator_pedals_error = errors_instance.accelerator_pedals_error & (~(1 << Error));
		break;
		case SEVEN_SEGMENT:
			errors_instance.seven_seg_error = errors_instance.seven_seg_error & (~(1<< Error));
		break;
		case PROCESSOR:
			errors_instance.processor_error = errors_instance.processor_error & (~(1<<Error));
		break;
		default:
			;
		}


}
