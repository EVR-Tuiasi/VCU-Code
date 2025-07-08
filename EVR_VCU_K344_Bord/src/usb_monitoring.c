#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "usb_monitoring.h"
#include "CDD_Uart.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

uint8 UART_Channel;
uint8 buffer[30];
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void USBInit(uint8 UartChannel){
	UART_Channel = UartChannel;
}

void USBSendCellTemperature(uint16 CellIndex, sint32 Value){
	buffer[0] = CELL_TEMP;
	buffer[1] = (CellIndex & 0xFF00) >> 8;
	buffer[2] = CellIndex & 0xFF;
	buffer[3] = ((uint32_t)(Value & 0xFF000000)) >> 24U;
	buffer[4] = ((uint32_t)(Value & 0x00FF0000)) >> 16U;
	buffer[5] = ((uint32_t)(Value & 0x0000FF00)) >> 8U;
	buffer[6] = (uint32_t)(Value & 0x000000FF);
	buffer[7] = CRC_calculate(8);
	Uart_SyncSend(UART_Channel, buffer, 8, 50000000);
}
void USBSendCellVoltage(uint16 CellIndex, sint32 Value){
	buffer[0] = CELL_VOLTAGE;
	buffer[1] = (CellIndex & 0xFF00) >> 8;
	buffer[2] = CellIndex & 0xFF;
	buffer[3] = ((uint32_t)(Value & 0xFF000000)) >> 24U;
	buffer[4] = ((uint32_t)(Value & 0x00FF0000)) >> 16U;
	buffer[5] = ((uint32_t)(Value & 0x0000FF00)) >> 8U;
	buffer[6] = (uint32_t)(Value & 0x000000FF);
	buffer[7] = CRC_calculate(8);
	Uart_SyncSend(UART_Channel, buffer, 8, 50000000);
}
void USBSendAcceleratorPedals(uint16 Value1, uint16 Value2){
	buffer[0] = ACCELERATOR_PEDALS;
	buffer[1] = (Value1 & 0xFF00) >> 8U;
	buffer[2] = Value1 & 0xFF;
	buffer[3] = (Value2 & 0xFF00) >> 8U;
	buffer[4] = Value2 & 0xFF;
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}
//void USBSendError(uint16 Module, uint8 error);
void USBSendInverterRPM(uint16 Inverter1rpm, uint16 Inverter2rpm){
	buffer[0] = INVERTERS_RPM;
	buffer[1] = (Inverter1rpm & 0xFF00) >> 8U;
	buffer[2] = Inverter1rpm & 0xFF;
	buffer[3] = (Inverter2rpm & 0xFF00) >> 8U;
	buffer[4] = Inverter2rpm & 0xFF;
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}
void USBSendInverterVoltage(uint16 Inverter1Voltage, uint16 Inverter2Voltage){
	buffer[0] = INVERTERS_VOLTAGE;
	buffer[1] = (Inverter1Voltage & 0xFF00) >> 8U;
	buffer[2] = Inverter1Voltage & 0xFF;
	buffer[3] = (Inverter2Voltage & 0xFF00) >> 8U;
	buffer[4] = Inverter2Voltage & 0xFF;
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}
void USBSendInverterThrottle(uint8 Inverter1Throttle, uint8 Inverter2Throttle){
	buffer[0] = INVERTERS_PEDALS;
	buffer[1] = Inverter1Throttle;
	buffer[2] = Inverter2Throttle;
	buffer[3] = CRC_calculate(4);
	Uart_SyncSend(UART_Channel, buffer, 4, 50000000);
}
void USBSendInverterControllerTemperature(uint8 Inverter1ControllerTemperature, uint8 Inverter2ControllerTemperature){
	buffer[0] = INVERTERS_CONTROLLER_TEMPERATURE;
	buffer[1] = Inverter1ControllerTemperature;
	buffer[2] = Inverter2ControllerTemperature;
	buffer[3] = CRC_calculate(4);
	Uart_SyncSend(UART_Channel, buffer, 4, 50000000);
}
void USBSendInverterMotorTemperature(uint8 Inverter1MotorTemperature, uint8 Inverter2MotorTemperature){
	buffer[0] = INVERTERS_MOTOR_TEMPERATURE;
	buffer[1] = Inverter1MotorTemperature;
	buffer[2] = Inverter2MotorTemperature;
	buffer[3] = CRC_calculate(4);
	Uart_SyncSend(UART_Channel, buffer, 4, 50000000);
}
void USBSendInverterCurrent(uint16 Inverter1Current, uint16 Inverter2Current){
	buffer[0] = INVERTERS_CURRENT;
	buffer[1] = (Inverter1Current & 0xFF00) >> 8U;
	buffer[2] = Inverter1Current & 0xFF;
	buffer[3] = (Inverter2Current & 0xFF00) >> 8U;
	buffer[4] = Inverter2Current & 0xFF;
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}
void USBSendBMSCurrent(sint32 Value){
	buffer[0] = BMS_CURRENT;
	buffer[1] = ((uint32_t)(Value & 0xFF000000)) >> 24U;
	buffer[2] = ((uint32_t)(Value & 0x00FF0000)) >> 16U;
	buffer[3] = ((uint32_t)(Value & 0x0000FF00)) >> 8U;
	buffer[4] = (uint32_t)(Value & 0x000000FF);
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}
void USBSendBMSVoltage(sint32 Value){
	buffer[0] = BMS_VOLTAGE;
	buffer[1] = ((uint32_t)(Value & 0xFF000000)) >> 24U;
	buffer[2] = ((uint32_t)(Value & 0x00FF0000)) >> 16U;
	buffer[3] = ((uint32_t)(Value & 0x0000FF00)) >> 8U;
	buffer[4] = (uint32_t)(Value & 0x000000FF);
	buffer[5] = CRC_calculate(6);
	Uart_SyncSend(UART_Channel, buffer, 6, 50000000);
}

void USBSendBrakePedal(uint16 Value){
	buffer[0] = BRAKE_PEDAL;
	buffer[1] = (Value & 0xFF00) >> 8U;
	buffer[2] = Value & 0xFF;
	buffer[3] = CRC_calculate(4);
	Uart_SyncSend(UART_Channel, buffer, 4, 50000000);
}

uint8 CRC_calculate(uint8 length){
	uint8 crc=0, message[length];
	uint16 divisor = 0x8D, dividend;
	int i, j;

	for(i=0; i<length-1; i++)
	{
		message[i] = buffer[i];
	}

	message[length-1]=0;

	dividend = (message[0] << 8) | message[1];
	for(j=15; j>=8; j--)
		if(dividend & (1 << j))
			dividend ^= divisor << (j-8);

	for(i=2; i<length; i++)
		{
			dividend = (dividend << 8) | message[i];

			for(j=15; j>=8; j--)
				if(dividend & (1 << j))
					dividend ^= divisor << (j-8);
		}

	crc = (dividend % 256);

	return crc;
}

#ifdef __cplusplus
}
#endif

/** @} */
