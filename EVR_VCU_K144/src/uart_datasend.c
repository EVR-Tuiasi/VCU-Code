#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "uart_datasend.h"
#include "CDD_Uart.h"
#include "uart_error_handling.h"

#define ERROR 9
#define MODULE_START 10
#define TEMP_SENSOR 10
#define BMS_VOLTAGE 11
#define BMS_CURRENT 12
#define ACCELERATOR_PEDALS 13
#define BRAKE_PEDAL 14
#define SEVEN_SEGMENT 15
#define PROCESSOR 16
#define MODULE_END 16

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

uint8 buff1[] = " :000.00;discharging  ";
uint8 buff2[] = " :00.00;discharging  ";
uint8 buff3[] = " :0.00;discharging  ";

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

uint8 UART_Channel;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



//parametrul volt pres. ca e int*100
void sendvolt(unsigned int volt)
{
	if(volt < 100000 && volt > 9999)
	{
		buff1[0] = 1 + '0';
		buff1[20] = 13;
		buff1[21]= 10;
		buff1[2] = volt/10000 + '0';
		buff1[3] = (volt/1000)%10 + '0';
		buff1[4] = (volt/100)%10 + '0';
		buff1[6] = (volt/10)%10 + '0';
		buff1[7] = volt%10 + '0';
		Uart_SyncSend(0, buff1, 22, 10000000);
	}

	else if(volt < 10000 && volt > 999)
	{
		buff2[0] = 1 + '0';
		buff2[19] = 13;
		buff2[20]= 10;
		buff2[2] = volt/1000 + '0';
		buff2[3] = (volt/100)%10 + '0';
		buff2[5] = (volt/10)%10 + '0';
		buff2[6] = volt%10 + '0';
		Uart_SyncSend(0, buff2, 21, 10000000);
	}

	else if(volt < 1000)
	{
		buff3[0] = 1 + '0';
		buff3[18] = 13;
		buff3[19]= 10;
		buff3[2] = volt/100 + '0';
		buff3[4] = (volt/10)%10 + '0';
		buff3[5] = volt%10 + '0';
		Uart_SyncSend(0, buff3, 20, 10000000);
	}

}

void USBInit(uint8 UartChannel){
	UART_Channel = UartChannel;
}
void USBSendCellTemperature(uint8 CellIndex, uint16 Value, uint8 Precision){
uint8 buffer[5];
buffer[0] = TEMP_SENSOR;
buffer[1] = CellIndex;
buffer[2] = Value >> 8;
buffer[3] = Value % 256;
buffer[4] = Precision;
Uart_SyncSend(UART_Channel, buffer, 5, 10000000);
}
void USBSendBMSCellVoltage(uint16 CellIndex, uint16 Value, uint8 Precision){
	uint8 buffer[6];
	buffer[0] = BMS_VOLTAGE;
	buffer[1] = CellIndex >> 8;
	buffer[2] = CellIndex % 256;
	buffer[3] = Value >> 8;
	buffer[4] = Value % 256;
	buffer[5] = Precision;
	Uart_SyncSend(UART_Channel, buffer, 6, 10000000);
}
void USBSendBMSCurrent(uint16 Value, uint8 Precision){
	uint8 buffer[4];
	buffer[0] = BMS_CURRENT;
	buffer[1] = Value >> 8;
	buffer[2] = Value % 256;
	buffer[3] = Precision;
	Uart_SyncSend(UART_Channel, buffer, 4, 10000000);
}
void USBSendAcceleratorPedals(uint16 Value1, uint16 Value2, uint8 Precision){
	uint8 buffer[6];
	buffer[0] = ACCELERATOR_PEDALS;
	buffer[1] = Value1 >> 8;
	buffer[2] = Value1 % 256;
	buffer[3] = Value2 >> 8;
	buffer[4] = Value2 % 256;
	buffer[5] = Precision;
	Uart_SyncSend(UART_Channel, buffer, 6, 10000000);
}
void USBSendBrakePedal(uint16 Value, uint8 Precision){
	uint8 buffer[4];
	buffer[0] = BRAKE_PEDAL;
	buffer[1] = Value >> 8;
	buffer[2] = Value % 256;
	buffer[3] = Precision;
	Uart_SyncSend(UART_Channel, buffer, 4, 10000000);
}

void USBSendErrors(void)
{
	for(int i = MODULE_START; i <= MODULE_END; i++)
	{
		uint8 aux = ErrorsGet(i);
		uint8 buffer[3];
		buffer[0] = ERROR;
		buffer[1] = i;
		buffer[2] = aux;
		Uart_SyncSend(UART_Channel, buffer, 3, 10000000);
	}
}




#ifdef __cplusplus
}
#endif

/** @} */
