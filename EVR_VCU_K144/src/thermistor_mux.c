#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "thermistor_mux.h"
#include "Dio.h"
#include "Port.h"
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

Thermistors Thermistors_Data;

// Nume temporare pt buffere
/*
uint16 bankselpins[THERMISTOR_BANKS] = {32+15,32+16,128+2,128+6,128+8,96+2,96+3,96+4,64+14,64+15,64+4,32+6,32+7,32+12,32+13,32+14},
		bankselpinsid[THERMISTOR_BANKS] = {64,63,84,83,25,70,69,68,45,44,95,15,14,67,66,65},
		adcreadchannels[THERMISTORS_PER_BANK] = {0, 1,2,3,4,5,6,7};*/

uint16 bankselpins[THERMISTOR_BANKS] = {64+14,64+15},
		bankselpinsid[THERMISTOR_BANKS] = {45,44},
		adcreadchannels[THERMISTORS_PER_BANK] = {0, 1,2,3};


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

static void ActivateThermistorBank(uint16 ThermistorBankIndex){
	Port_SetPinDirection(Thermistors_Data.BankSelectPinsID[ThermistorBankIndex], PORT_PIN_OUT);
	Dio_WriteChannel(Thermistors_Data.BankSelectPins[ThermistorBankIndex], STD_LOW);
}

static void DeactivateThermistorBank(uint16 ThermistorBankIndex){
	Port_SetPinDirection(Thermistors_Data.BankSelectPinsID[ThermistorBankIndex], PORT_PIN_HIGH_Z);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void TempSensorInit(){
	for(int i = 0; i < THERMISTOR_BANKS; i++){
		for(int j = 0; j < THERMISTORS_PER_BANK; j++){
			Thermistors_Data.ThermistorValues[i][j] = 0;
			Thermistors_Data.BankReadChannels[j] = adcreadchannels[j];
		}
		Thermistors_Data.BankSelectPins[i] = bankselpins[i];
		Thermistors_Data.BankSelectPinsID[i] = bankselpinsid[i];
	}

	for(int i = 0; i < THERMISTORS_PER_BANK; i++){
		DeactivateThermistorBank(i);
	}
}

sint32 GetTemp(uint16 TempSensorIndex){
	ActivateThermistorBank(TempSensorIndex);

	for(int i = 0; i < THERMISTORS_PER_BANK; i++){
		Adc_SetupResultBuffer(Thermistors_Data.BankReadChannels[i], &Thermistors_Data.ThermistorValues[TempSensorIndex][i]);
		Adc_StartGroupConversion(Thermistors_Data.BankReadChannels[i]);

		//while(Adc_GetGroupStatus(Thermistors_Data.BankReadChannels[i]) != ADC_STREAM_COMPLETED);

		Adc_ReadGroup(Thermistors_Data.BankReadChannels[i], &Thermistors_Data.ThermistorValues[TempSensorIndex][i]);
	}

	DeactivateThermistorBank(TempSensorIndex);

	// Probabil calcul matematic pentru temperatura

	return 0;
}

void TempSensorTest(){
	;
}

#ifdef __cplusplus
}
#endif

/** @} */
