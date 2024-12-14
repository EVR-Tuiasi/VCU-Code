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

uint8 data0[2] = {12, 1}; // comanda normal mode
uint8 data1[2] = {10, 15}; // comanda luminozitate maxima globala
uint8 data2[2] = {1,8}; // comanda prima cifra pe 8


I2c_RequestType req0 = {0, false, false, false, false, 2, I2C_SEND_DATA, data0};
I2c_RequestType req1 = {0, false, false, false, false, 2, I2C_SEND_DATA, data1};
I2c_RequestType req2 = {0, false, false, false, false, 2, I2C_SEND_DATA, data2};

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
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
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

    while(1){
    	volatile int p = 100000;
    	//if(ok){
    		while(p != 0)
    			p--;
			I2c_SyncTransmit(0, &req0);
			I2c_SyncTransmit(0, &req1);
			I2c_SyncTransmit(0, &req2);
			ok = 0;
    	//}
    }
}


#ifdef __cplusplus
}
#endif

/** @} */
