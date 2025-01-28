#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "CDD_I2c.h"
#include "Dio.h"
#include "Icu.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
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

uint8 DigitNumar1[2] = {0x01, 0x03};
uint8 DigitNumar2[2] = {0x02, 0x0e};
uint8 DigitNumar3[2] = {0x03, 0x02};
uint8 DigitNumar4[2] = {0x04, 0x08};// numarul care va fi afisat pe digit
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
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
	Dio_WriteChannel(96, 1);
}

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
    Port_Init(NULL_PTR);
    Platform_Init(NULL_PTR);
    I2c_Init(NULL_PTR);
    Icu_Init(NULL_PTR);

    Icu_EnableNotification(0);
    SevenSegmentInit();

    volatile int i = 1;

    while(1){
    	volatile int p = 1000000;

    	while(p != 0)
    		p--;
    	//if(ok == 1){
    		if(i == 100)
    	    	i = 1;
    		SevenSegmentDisplayDecimalValue(0, 245, 2);
    		SevenSegmentSetGlobalBrightness(i);
    		i++;
      		//ok = 0;
    	//}
    	//I2c_SyncTransmit(0, &test);

		/*I2c_SyncTransmit(0, &numarpedigit4);
		I2c_SyncTransmit(0, &numarpedigit3);
		I2c_SyncTransmit(0, &numarpedigit2);
		I2c_SyncTransmit(0, &numarpedigit1);
*/
    }
}


#ifdef __cplusplus
}
#endif

/** @} */
