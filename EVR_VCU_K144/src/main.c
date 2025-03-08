#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "7-segment-display.h"
#include "CDD_I2c.h"
#include "Dio.h"
#include "Gpt.h"
#include "Icu.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"



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
	Dio_WriteChannel(96, 1);
	Dio_WriteChannel(111, 1);
}

void I2c_Callback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(96, 0);
	Dio_WriteChannel(111, 1);

	SevSegInteruptFunc();
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
	Dio_WriteChannel(96, 1);

	SevSegInteruptFunc();
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
    Gpt_Init(NULL_PTR);
    I2c_Init(NULL_PTR);
    Icu_Init(NULL_PTR);

    Gpt_EnableNotification(0);
    Icu_EnableNotification(0);

    SevenSegmentInit();

    while(1){
    	//volatile int i = 9999;

    	/*while(1){
    		SevenSegmentDisplayDecimalValue(0, i, 2);

    		i--;

    		UpdateState();
    	}*/

    	SevSegGrTest(0);

    }
}


#ifdef __cplusplus
}
#endif

/** @} */
