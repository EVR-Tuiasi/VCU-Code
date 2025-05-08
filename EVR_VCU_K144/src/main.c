#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Adc.h"
#include "Mcl.h"
#include "CDD_I2c.h"
#include "Dio.h"
#include "Icu.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "uart_datasend.h"
#include "uart_error_handling.h"
#include "CDD_Uart.h"
#include "7-segment-display.h"


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

uint8 DigitNumar1[2] = {0x01, 0x0f};
uint8 DigitNumar2[2] = {0x02, 0x0f};
uint8 DigitNumar3[2] = {0x03, 0x0f};
uint8 DigitNumar4[2] = {0x04, 0x0f};// numarul care va fi afisat pe digit
uint8 test_data[2] = {0x0f, 1}; // comanda test optic

I2c_RequestType test = {0, false, false, false, false, 2, I2C_SEND_DATA, test_data};
I2c_RequestType numarpedigit4 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar4};
I2c_RequestType numarpedigit3 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar3};
I2c_RequestType numarpedigit2 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar2};
I2c_RequestType numarpedigit1 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar1};

volatile uint8 ok = 0;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void IntrerupereBTN(void){
	ok = 1;
	Dio_WriteChannel(96, 1);
	Dio_WriteChannel(111, 1);

}

void I2c_Callback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(96, 0);
	Dio_WriteChannel(111, 1);

	(void)Event;
	(void)Channel;
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
	Dio_WriteChannel(96, 1);
	ok = 1;

	(void)Event;
	(void)Channel;
}

#define PIN_BUF_SIZE 3
#define ADC_COUNT 2

int main(void)
{

    /* Initialize the Mcu driver */
#if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
#elif (MCU_PRECOMPILE_SUPPORT == STD_OFF)
    Mcu_Init(&Mcu_Config_VS_0);
#endif /* (MCU_PRECOMPILE_SUPPORT == STD_ON) */

    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);
#if (MCU_NO_PLL == STD_OFF)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }

    Mcu_DistributePllClock();
#endif
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize all pins using the Port driver */
    Mcl_Init(NULL_PTR);
    Port_Init(NULL_PTR);
    Platform_Init(NULL_PTR);
    Adc_Init(NULL_PTR);
    Uart_Init(NULL_PTR);
    I2c_Init(NULL_PTR);
    Icu_Init(NULL_PTR);
    Icu_EnableNotification(0);

    //USBInit(0);
    //SevenSegmentInit();
    //SevSegGrTest(0);
    //ErrorsSet(BMS_VOLTAGE, BMS_NO_RESPONSE);
    //ErrorsSet(SEVEN_SEGMENT, SEVEN_SEG_NO_RESPONSE);
    //ErrorsSet(SEVEN_SEGMENT, SEVEN_SEG_NUMBER_TOO_LARGE);
    //ErrorsSet(BRAKE_PEDAL, ACCELERATOR_PEDALS_DIFFERENT_OUTPUT);
    //ErrorsSet(BMS_CURRENT, BMS_NO_RESPONSE);

    uint16 rezultat_buffer[PIN_BUF_SIZE][ADC_COUNT];
	uint16 grounds[PIN_BUF_SIZE * 2] = {6, 7, 8, 67, 2, 3};

	for(int i = 0; i < PIN_BUF_SIZE; i++)
		Port_SetPinDirection(grounds[i], PORT_PIN_HIGH_Z);

	for(int i = 0; i < ADC_COUNT; i++){
		for(int k = 0; k < PIN_BUF_SIZE; k++){
			Port_SetPinDirection(grounds[k], PORT_PIN_OUT);
			Dio_WriteChannel(grounds[k + PIN_BUF_SIZE], STD_LOW);

			Adc_SetupResultBuffer(i, &rezultat_buffer[k][i]);
			Adc_StartGroupConversion(i);

			while(Adc_GetGroupStatus(i) != ADC_STREAM_COMPLETED);

			Adc_ReadGroup(i, &rezultat_buffer[k][i]);

			rezultat_buffer[k][i] = (rezultat_buffer[k][i] * 500) / 4095;
			Port_SetPinDirection(grounds[k], PORT_PIN_HIGH_Z);
		}
	}

	while(1){
		;
	}


//    while(1){
//    	volatile int i = 100000;
//    	while(i)
//    		i--;
//    	USBSendErrors();
//    }
}
// test

#ifdef __cplusplus
}
#endif

/** @} */
