
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
#include "Pwm.h"
#include "Can_GeneralTypes.h"
#include "Can_43_FLEXCAN.h"
#include "CanIf.h"
#include "SchM_Can_43_FLEXCAN.h"
#include "CDD_Uart.h"

#include "display.h"
#include "FT81_misc.h"
#include "FT81_display.h"
#include "FT81_sound.h"
#include "FT81_touch.h"
#include "7-segment-display.h"
#include "pedals.h"
#include "Dac.h"
#include "invertor.h"
#include "usb_monitoring.h"

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
/**
* @brief        Main function of the example
* @details      Initialize the used drivers and uses the Icu
*               and Dio drivers to toggle a LED on a push button
*/
int main(void)
{
    Mcu_Init(NULL_PTR);
    Mcu_InitClock(McuClockSettingConfig_0);
    while(MCU_PLL_LOCKED != Mcu_GetPllStatus())
    {
    	;
    }
    Mcu_DistributePllClock();
    Mcu_SetMode(McuModeSettingConf_0);
    Platform_Init(NULL_PTR);
    Port_Init(NULL_PTR);
    Mcl_Init(NULL_PTR);
    Gpt_Init(NULL_PTR);
    Spi_Init(NULL_PTR);
    I2c_Init(NULL_PTR);
    Adc_Init(NULL_PTR);
    Pwm_Init(NULL_PTR);
    Uart_Init(NULL_PTR);
	Can_43_FLEXCAN_Init(NULL_PTR);
	CanIf_Init(NULL_PTR);

    /* Wdg_43_fs26 initialization */
    volatile Std_ReturnType eReturnValue = E_OK;      /* Error status. */
    //eReturnValue |= Sbc_fs26_Init(NULL_PTR);
    //Wdg_43_fs26_Init(NULL_PTR);

    //eReturnValue |= Sbc_fs26_InitDevice();
    //eReturnValue |= Wdg_43_fs26_SetMode(WDGIF_OFF_MODE);
    if(eReturnValue == E_OK){
    	Dio_WriteChannel(140, 0);
    	Dio_WriteChannel(142, 1);
    }
    else{
    	Dio_WriteChannel(140, 1);
    	Dio_WriteChannel(142, 0);
    }

    SevenSegmentInit();
    //SevenSegmentTest();
	DisplayInit();
	PedalsInit();
	DacInit();
    InverterInit();
    USBInit(0);
	DacEnable();
	//DisplayTest();
	//DashboardTest();
	//SoundTest();
	volatile uint32 frana = 0, acceleratie = 0, rpm = 0, tensiune = 0, curent = 0, tempController = 0, tempMotor = 0, putere = 0, procentaj = 0, tempMaxim = 0, viteza = 0, throttle = 0;
	while(1){
		//citire valori senzori frana
		frana = PedalsGetBrakePercent();
		acceleratie = PedalsGetAccelerationPercent();

		//TODO implementare BSPD

		//modificare output comanda de cuplu
		DacSetOutput(0, acceleratie);
		DacSetOutput(1, acceleratie);

		//citire date de la invertor
		//InverterUpdate();
	    //Can_43_FLEXCAN_MainFunction_Read();
	    //Can_43_FLEXCAN_MainFunction_Read();
        rpm = InverterGetRpm(0);
        curent = InverterGetCurrent(0);//curent returnat cu o virgula
        tensiune = InverterGetVoltage(0);//tensiune returnata cu o virgula
        tempController = InverterGetControllerTemperature(0);
        tempMotor = InverterGetMotorTemperature(0);
        throttle = InverterGetThrottle(0);

        //calcul putere instantanee
        if(curent != 0 && tensiune != 0){
            putere = (((uint32)curent/10U) * ((uint32)tensiune/10U));
        }
        else{
        	putere = 0U;
        }

		//actualizare afisaje segmente
        //calcul procentaj baterie
        if(tensiune < 600U){
            procentaj = 0;
        }
        else if(tensiune > 1000U){
        	procentaj = 1000U;//procentaj calculat cu o virgula
        }
        else{
        	procentaj = (uint16)(tensiune - 600U) * 5U / 2U;//procentaj calculat cu o virgula
        }
        if(procentaj < 1000U){
            SevenSegmentDisplayDecimalValue(2, procentaj, 1);
        }
        else{
            SevenSegmentDisplayDecimalValue(2, procentaj/10U, 0);
        }
        tempMaxim = 0;
        if(tempController > tempMotor){
        	tempMaxim = tempController;
        }
        else{
        	tempMaxim = tempMotor;
        }
        SevenSegmentDisplayDecimalValue(0, tempMaxim, 0);
        viteza = 0;
        if(rpm != 0){
        	viteza = (rpm * 84807U) / 312500U;
        }
        if(viteza < 1000U){
            SevenSegmentDisplayDecimalValue(1, viteza, 1);
        }
        else{
            SevenSegmentDisplayDecimalValue(1, viteza/10U, 0);
        }
		//actualizare interfata display
		//TODO martori de bord
        DashboardUpdate(rpm, putere, tensiune/10U, procentaj/10U, tempMotor, tempController, frana, acceleratie);
        //trimitere date pe uart
        USBSendAcceleratorPedals(PedalsGetAccelerationPercentSensor1(), PedalsGetAccelerationPercentSensor2());
        USBSendBrakePedal(frana);
        USBSendInverterRPM(rpm, 0);
        USBSendInverterVoltage(tensiune,0);
        USBSendInverterCurrent(curent, 0);
        USBSendInverterThrottle(throttle, 0);
        USBSendInverterControllerTemperature(tempController, 0);
		USBSendInverterMotorTemperature(tempMotor, 0);
	}

	while(1);


}


#ifdef __cplusplus
}
#endif

/** @} */
