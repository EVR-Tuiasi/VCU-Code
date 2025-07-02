
#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Sbc_fs26.h"
#include "Port.h"
#include "Det.h"
#include "Dem.h"
#include "Spi.h"
#include "Platform.h"
#include "Wdg_43_fs26_CfgExt.h"
#include "Wdg_43_fs26_Cfg.h"
#include "Wdg_43_fs26.h"
#include "Gpt.h"
#include "Mcu.h"
#include "Dio.h"
#include "Mcl.h"
#include "CDD_I2c.h"
#include "Adc.h"

#include "display.h"
#include "FT81_misc.h"
#include "FT81_display.h"
#include "FT81_sound.h"
#include "FT81_touch.h"
#include "7-segment-display.h"
#include "pedals.h"

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
void I2c_Callback(uint8 Event, uint8 Channel){
	;
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



void TestDelay(uint32 delay);
void TestDelay(uint32 delay)
{
    static volatile uint32 DelayTimer = 0;
    while(DelayTimer < delay)
    {
        DelayTimer++;
    }
    DelayTimer = 0;
}

/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Icu
*               and Dio drivers to toggle a LED on a push button
*/
int main(void)
{
    /* Initialize the Mcu driver */
    Mcu_Init(NULL_PTR);

    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);

    while(MCU_PLL_LOCKED != Mcu_GetPllStatus())
    {
    	;
    }
    Mcu_DistributePllClock();
    /* Apply a mode configuration */
    Mcu_SetMode(McuModeSettingConf_0);

    /* Platform initialization */
    Platform_Init(NULL_PTR);

    /* Port initialization */
    Port_Init(NULL_PTR);

    Mcl_Init(NULL_PTR);

    /* GPT initialization */
    Gpt_Init(NULL_PTR);

    /* SPI initialization */
    Spi_Init(NULL_PTR);

    /*I2c initialization */
    I2c_Init(NULL_PTR);

    Adc_Init(NULL_PTR);

    /* Wdg_43_fs26 initialization */
    volatile Std_ReturnType eReturnValue = E_OK;      /* Error status. */
    eReturnValue |= Sbc_fs26_Init(NULL_PTR);
    Wdg_43_fs26_Init(NULL_PTR);

    eReturnValue |= Sbc_fs26_InitDevice();
    eReturnValue |= Wdg_43_fs26_SetMode(WDGIF_OFF_MODE);
    if(eReturnValue == E_OK){
    	Dio_WriteChannel(140, 0);
    	Dio_WriteChannel(142, 1);
    }
    else{
    	Dio_WriteChannel(140, 1);
    	Dio_WriteChannel(142, 0);
    }

    //SevenSegmentInit();
    //SevSegGrTest(0);
	DisplayInit();
	PedalsInit();
	//DisplayTest();
	//DashboardTest();
	//VladTest();
	//VladTest();
	//SoundTest();
	while(1){
		volatile uint16 frana = PedalsGetBrakePercent();
		volatile uint16 acceleratie = PedalsGetAccelerationPercent();
		DashboardUpdate(0, 0, 0, 0, 0, 0, frana, acceleratie);
	}

	while(1);


}


#ifdef __cplusplus
}
#endif

/** @} */
