
#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include"Mcu.h"
#include"pedals.h"
#include"Gpt.h"
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
Pedals Pedalsinstance = {
	1,0,2,  //grupuri
	0,0,0,  //valori
};
uint16 brakeDebounceBuffer[BRAKE_DEBOUNCE_BUFFER_SIZE] = {0U};
uint8 brakeDebounceIndex = 0U;

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
void Gpt_Pedals(void){
	if(Adc_GetGroupStatus(Pedalsinstance.BrakeAdcGroup) != ADC_BUSY){
		Adc_StartGroupConversion(Pedalsinstance.BrakeAdcGroup);
	}
}

void Adc_Frana(void){
	uint16 pedalValue = Pedalsinstance.BrakeValue;
	uint64 mean = 0;
	//teste limite
	if((pedalValue < BRAKE_START_LIMIT) || (pedalValue > BRAKE_END_LIMIT)){
		pedalValue = BRAKE_START_VALID;
	}
	else{
		if((pedalValue >= BRAKE_START_LIMIT) && (pedalValue <= BRAKE_START_VALID)){
			pedalValue = BRAKE_START_VALID;
		}
		else if((pedalValue >= BRAKE_END_VALID) && (pedalValue <= BRAKE_END_LIMIT)){
			pedalValue = BRAKE_END_VALID;
		}
		else{
			pedalValue = Pedalsinstance.BrakeValue;
		}
	}
	//inversare valoare
	pedalValue = BRAKE_START_VALID + (BRAKE_END_VALID - pedalValue);
	//debounce
	brakeDebounceBuffer[brakeDebounceIndex] = pedalValue;
	brakeDebounceIndex = (brakeDebounceIndex + 1) % BRAKE_DEBOUNCE_BUFFER_SIZE;
	for(uint8 i=0; i<BRAKE_DEBOUNCE_BUFFER_SIZE; i++){
		mean += brakeDebounceBuffer[i];
	}
	mean /= BRAKE_DEBOUNCE_BUFFER_SIZE;
	Pedalsinstance.BrakeValue = mean;
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void PedalsInit(void)
{
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcGroup1, &Pedalsinstance.AccelerationValue1);
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcGroup2, &Pedalsinstance.AccelerationValue2);
	Adc_SetupResultBuffer(Pedalsinstance.BrakeAdcGroup, &Pedalsinstance.BrakeValue);
	Adc_EnableGroupNotification(2U);
	Gpt_EnableNotification(1U);
	Gpt_StartTimer(1U, 40000U);
}

uint16 PedalsGetAccelerationPercentSensor1(void){
	uint16 pedalPercent = (((uint32)(Pedalsinstance.AccelerationValue1 - ACCEL_1_START_VALID)) * 100U) / (ACCEL_1_END_VALID - ACCEL_1_START_VALID);
	return pedalPercent;
}

uint16 PedalsGetAccelerationPercentSensor2(void){
	uint16 pedalPercent = (((uint32)(Pedalsinstance.AccelerationValue2 - ACCEL_2_START_VALID)) * 100U) / (ACCEL_2_END_VALID - ACCEL_2_START_VALID);
	return pedalPercent;
}


uint16 PedalsGetAccelerationPercent(void)
{
	uint16 pedalPercent1 = 0, pedalPercent2 = 0, pedalValue1 = 0, pedalValue2 = 0;
	volatile Adc_StatusType statusAdc;
	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcGroup1);
	do{
		statusAdc = Adc_GetGroupStatus(Pedalsinstance.AccelerationAdcGroup1);
	}while(statusAdc != ADC_STREAM_COMPLETED);
	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcGroup2);
	do{
		statusAdc = Adc_GetGroupStatus(Pedalsinstance.AccelerationAdcGroup2);
	}while(statusAdc != ADC_STREAM_COMPLETED);
	//teste limite senzor 1
	pedalValue1 = Pedalsinstance.AccelerationValue1;
	if((pedalValue1 < ACCEL_1_START_LIMIT) || (pedalValue1 > ACCEL_1_END_LIMIT)){
		pedalValue1 = ACCEL_1_START_VALID;
	}
	else{
		if((pedalValue1 >= ACCEL_1_START_LIMIT) && (pedalValue1 <= ACCEL_1_START_VALID)){
			pedalValue1 = ACCEL_1_START_VALID;
		}
		else if((pedalValue1 >= ACCEL_1_END_VALID) && (pedalValue1 <= ACCEL_1_END_LIMIT)){
			pedalValue1 = ACCEL_1_END_VALID;
		}
		else{
			pedalValue1 = Pedalsinstance.AccelerationValue1;
		}
	}
	Pedalsinstance.AccelerationValue1 = pedalValue1;
	//teste limite senzor 2
	pedalValue2 = Pedalsinstance.AccelerationValue2;
	if((pedalValue2 < ACCEL_2_START_LIMIT) || (pedalValue2 > ACCEL_2_END_LIMIT)){
		pedalValue2 = ACCEL_2_START_VALID;
	}
	else{
		if((pedalValue2 >= ACCEL_2_START_LIMIT) && (pedalValue2 <= ACCEL_2_START_VALID)){
			pedalValue2 = ACCEL_2_START_VALID;
		}
		else if((pedalValue2 >= ACCEL_2_END_VALID) && (pedalValue2 <= ACCEL_2_END_LIMIT)){
			pedalValue2 = ACCEL_2_END_VALID;
		}
		else{
			pedalValue2 = Pedalsinstance.AccelerationValue2;
		}
	}
	//inversare valoare
	pedalValue1 = ACCEL_1_START_VALID + (ACCEL_1_END_VALID - pedalValue1);
	Pedalsinstance.AccelerationValue1 = pedalValue1;
	//calculare valoare procentuala
	pedalPercent1 = (((uint32)(pedalValue1 - ACCEL_1_START_VALID)) * 100U) / (ACCEL_1_END_VALID - ACCEL_1_START_VALID);
	pedalPercent2 = (((uint32)(pedalValue2 - ACCEL_2_START_VALID)) * 100U) / (ACCEL_2_END_VALID - ACCEL_2_START_VALID);
	return (pedalPercent1 + pedalPercent2) / 2U;
	//return pedalPercent1;
}

uint16 PedalsGetBrakePercent(void){
	uint16 pedalPercent = 0;
	//calcul procent
	Adc_DisableGroupNotification(2U);
	pedalPercent = (((uint32)(Pedalsinstance.BrakeValue - BRAKE_START_VALID)) * 100U) / (BRAKE_END_VALID - BRAKE_START_VALID);
	Adc_EnableGroupNotification(2U);
	return pedalPercent;
}

void PedalsTest(void){
	;
}


#ifdef __cplusplus
}
#endif
/** @} */
