
#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Mcu.h"
#include "Port.h"
#include "pedal.h"
#include "Adc.h"
#include "Platform.h"
#include "Pwm.h"

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
#define PWM_MAX_DUTY 65535

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

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
    Pwm_Init(NULL_PTR);
    Adc_Init(NULL_PTR);
    PedalsInit();


    Pwm_SetDutyCycle ( 0,2457 );

    while(1){
    //	volatile uint8 acc_value = PedalsGetAcceleration();
    //	volatile uint8 brake_value = PedalsGetBrake();


        /* Calcularea duty cycle-ului pentru fiecare motor */
        uint16 motor1_duty = (PWM_MAX_DUTY * accel_value) / 100;
        uint16 motor2_duty = (PWM_MAX_DUTY * accel_value) / 100;

        /* Aplica franarea daca e cazul */
        if (brake_value > 0)
        {
            motor1_duty = 0;
            motor2_duty = 0;
        }

        /* Setarea PWM pentru motoare */
        Pwm_SetDutyCycle(0, motor1_duty); // Motor 1
        Pwm_SetDutyCycle(1, motor2_duty); // Motor 2
    }
}


// 2 inturi motor 1 si motor 2 si o valoare care reprezinta 0-100%. pe asta il transform in duty cicle si cu pwm_setdutycicle
// setez viteza pt motoare.


#ifdef __cplusplus
}
#endif

/** @} */
