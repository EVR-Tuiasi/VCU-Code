#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "pedal.h"
#include "Adc.h"

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
Pedals Pedalsinstance={ //initializari
		0,1,2,  //channeluri
		0,0,0,  //valori
		10   //eroare
};



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

void PedalsInit(void)
{
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel1, &Pedalsinstance.AccelerationValue1);
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel2, &Pedalsinstance.AccelerationValue2);
	Adc_SetupResultBuffer(Pedalsinstance.BrakeAdcChannel, &Pedalsinstance.BrakeValue);
}

uint8 PedalsGetAcceleration(void)
{
	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcChannel1);
	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcChannel2);
	Adc_StartGroupConversion(Pedalsinstance.BrakeAdcChannel);
	while(1){
	volatile	Adc_StatusType StatusAdc = Adc_GetGroupStatus(Pedalsinstance.AccelerationAdcChannel1);
		if (StatusAdc== ADC_STREAM_COMPLETED)
		{ break; }

       //Std_ReturnType in loc de unit8????
	volatile unit8 adcValue = Adc_ReadGroup(Pedalsinstance.AccelerationAdcChannel1,&Pedalsinstance.AccelerationValue1);  // citeste valoarea ADC
	volatile float percentage = (adcValue * 100.0f) / 255.0f;  // convert in procentaj
	Pedalsinstance.AccelerationValue1 = percentage;
	}
	return Pedalsinstance.AccelerationValue1;

}


uint8 PedalsGetBrake(void)
{ Adc_StartGroupConversion(Pedalsinstance.BrakeAdcChannel);
while(1){
volatile	Adc_StatusType StatusAdc = Adc_GetGroupStatus(Pedalsinstance.BrakeAdcChannel);
	if (StatusAdc== ADC_STREAM_COMPLETED)
	{ break; }

   }

return  Pedalsinstance.BrakeAdcChannel;
}

void PedalsTest(void){

;
}



#ifdef __cplusplus
}
#endif

/** @} */
