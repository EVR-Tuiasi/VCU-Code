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
#define TIME 17
#define TEMP_ALL 18
#define MODULE_END 19

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
volatile BUS_STATE state = BUS_IDLE;
uint8 buffer[258];
uint8 buffer_end[2];

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

void intrerupere_uart(void)
{
	state = BUS_IDLE;
	/*if (brake_pedal_changed == true)
			USBSendBrakePedal(brake_pedal_value, brake_pedal_precision);*/
}

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
	if(state == BUS_IDLE)
	{
		buffer[0] = TEMP_SENSOR;
		buffer[1] = CellIndex;
		buffer[2] = Value >> 8;
		buffer[3] = Value % 256;
		buffer[4] = Precision;
		state = BUS_BUSY;
		Uart_AsyncSend(UART_Channel, buffer, 5);
	}
}
void USBSendBMSCellVoltage(uint16 CellIndex, uint16 Value, uint8 Precision){
	buffer[0] = BMS_VOLTAGE;
	buffer[1] = CellIndex >> 8;
	buffer[2] = CellIndex % 256;
	buffer[3] = Value >> 8;
	buffer[4] = Value % 256;
	buffer[5] = Precision;
	Uart_AsyncSend(UART_Channel, buffer, 6);
}
void USBSendBMSCurrent(uint16 Value, uint8 Precision){
	buffer[0] = BMS_CURRENT;
	buffer[1] = Value >> 8;
	buffer[2] = Value % 256;
	buffer[3] = Precision;
	Uart_AsyncSend(UART_Channel, buffer, 4);
}
void USBSendAcceleratorPedals(uint16 Value1, uint16 Value2, uint8 Precision){
	buffer[0] = ACCELERATOR_PEDALS;
	buffer[1] = Value1 >> 8;
	buffer[2] = Value1 % 256;
	buffer[3] = Value2 >> 8;
	buffer[4] = Value2 % 256;
	buffer[5] = Precision;
	buffer[6] = CRC_calculate(7);
	Uart_AsyncSend(UART_Channel, buffer, 7);
}
void USBSendBrakePedal(uint16 Value, uint8 Precision){
	if(state == BUS_IDLE)
	{
		buffer[0] = BRAKE_PEDAL;
		buffer[1] = Value >> 8;
		buffer[2] = Value % 256;
		buffer[3] = Precision;
		buffer[4] = CRC_calculate(5);
		state = BUS_BUSY;
		Uart_AsyncSend(UART_Channel, buffer, 5);
	}
}

void USBSendErrors(void)
{
	buffer[0] = 0;
	for(int i = MODULE_START; i <= MODULE_END; i++)
	{
		uint8 aux = ErrorsGet(i);
		buffer[1] = ERROR;
		buffer[2] = i;
		buffer[3] = aux;
		Uart_SyncSend(UART_Channel, buffer, 4, 1000000);
	}
}

void USBTempTotal(uint8 Precision, uint16* Value)
{
	int i;
	buffer[0] = 0;
	buffer[1] = Precision;

	for(i=0; i<256; i+=2)
	{
		/*
		 *  codul pentru CRC de la sender
		 *
		 * uint16 aux = Value[i/2] << 3;
		 div =<< 12;

		 for(j=15; j>=3; j--)
		 	 {
		 	 	 if(aux & (1 << i))
		 	 	 	 aux ^= div;

		 	 	 	 div >>= 1;
		 	 }
		 	 crc = aux % 256;

		 	 buffer[258] --> buffer[384] maybe?
		  */

		uint16 temp_value = Value[i/2];
		buffer[i+2] = temp_value >> 8;
		buffer[i+3] = temp_value % 256;
	}

	Uart_AsyncSend(UART_Channel, buffer, 258);
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

	/*for(i=0; i<length; i++)
		message[i] = 0;*/

	return crc;
}

//se opreste la buffer[229] -> 210 .. buffer[230] -> 11 ..


#ifdef __cplusplus
}
#endif

/** @} */
