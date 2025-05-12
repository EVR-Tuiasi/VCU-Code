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

Thermistors thermistor = {
		{{0},{0}},
		{0},
		{0},
		{0}
};

// Nume temporare pt buffere

uint16 bankselpins[THERMISTOR_BANKS] = {67, 2, 3},
		bankselpinsid[THERMISTOR_BANKS] = {6, 7, 8},
		adcreadchannels[THERMISTORS_PER_BANK] = {0, 1};


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
	(void)ThermistorBankIndex;
}

static void DeactivateThermistorBank(uint16 ThermistorBankIndex){
	(void)ThermistorBankIndex;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void TempSensorInit(){
	for(int i = 0; i < THERMISTOR_BANKS; i++){
		for(int j = 0; j < THERMISTORS_PER_BANK; j++){
			thermistor.ThermistorValues[i][j] = 0;
			thermistor.BankReadChannels[j] = adcreadchannels[j];
		}
		thermistor.BankSelectPins[i] = bankselpins[i];
		thermistor.BankSelectPinsID[i] = bankselpinsid[i];
	}

	for(int i = 0; i < THERMISTORS_PER_BANK; i++){
		Port_SetPinDirection(thermistor.BankSelectPinsID[i], PORT_PIN_HIGH_Z);
	}
}

sint32 GetTemp(uint16 TempSensorIndex){
	ActivateThermistorBank(TempSensorIndex);

	// Logica Functie

	DeactivateThermistorBank(TempSensorIndex);
	return 0;
}

void TempSensorTest(){
	;
}

#ifdef __cplusplus
}
#endif

/** @} */
