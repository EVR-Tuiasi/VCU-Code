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
#include "bms_cosa.h"


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
//uint16 bankselpins[THERMISTOR_BANKS] =    {32,33,38,39,44,45,46,64,65,66,67,100,78,80,81,98}, //alocare initiala
uint16 bankselpins[THERMISTOR_BANKS] =    {39,38,100,46,44,32,33,64,67,78,45,65,66,80,81,98},   //alocare final
//		bankselpinsid[THERMISTOR_BANKS] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33},	//alocare init
		bankselpinsid[THERMISTOR_BANKS] = {21,20,29,24,22,18,19,25,28,30,23,26,27,31,32,33},	//alocare finala

		//adcreadchannels[THERMISTORS_PER_BANK] = {8,9,2,3,4,5,6,7}; //ordinea initiala
		adcreadchannels[THERMISTORS_PER_BANK] = {4,2,3,8,9,6,7,5}; //ordine de oameni normali, verifica PINOUT !!!
extern struct biemese icBaterie;

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
		volatile Adc_StatusType temp;
		do
			{
			temp=Adc_GetGroupStatus(Thermistors_Data.BankReadChannels[i]);
			}
		while(temp != ADC_STREAM_COMPLETED);

		//Adc_GetGroupStatus(Thermistors_Data.BankReadChannels[i])
		Adc_ReadGroup(Thermistors_Data.BankReadChannels[i], &Thermistors_Data.ThermistorValues[TempSensorIndex][i]);
	}

	DeactivateThermistorBank(TempSensorIndex);

	// Probabil calcul matematic pentru temperatura

	return 0;
}

void TempSensorTest(){
	;
}

void corectieTemperatura(){
	for(int i=0;i<THERMISTOR_BANKS;i++)
	    	{
	    		for(int j=0;j<THERMISTORS_PER_BANK;j++)
	    		{
	    			if(Thermistors_Data.ThermistorValues[i][j]<1040)
	    				Thermistors_Data.ThermistorValues[i][j]=1040;
	    			else if(Thermistors_Data.ThermistorValues[i][j]>2074)
	    				Thermistors_Data.ThermistorValues[i][j]=2074;

	    			if(Thermistors_Data.ThermistorValues[i][j]>=1140)
	    			{
	    				Thermistors_Data.ThermistorValues[i][j]=68400/Thermistors_Data.ThermistorValues[i][j];
	    				//60 65
	    			}
	    			else if(Thermistors_Data.ThermistorValues[i][j]>=1248)
	    			{
	    				//55 60
	    				Thermistors_Data.ThermistorValues[i][j]=68640/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    			else if(Thermistors_Data.ThermistorValues[i][j]>=1399)
	    			{
	    			    				//55 50
	    				Thermistors_Data.ThermistorValues[i][j]=69950/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    			else if(Thermistors_Data.ThermistorValues[i][j]>=1545)
	    			{
	    			    				//50 45
	    				Thermistors_Data.ThermistorValues[i][j]=69525/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    			else if(Thermistors_Data.ThermistorValues[i][j]>=1708)
	    			{
	    			    				//45 40
	    				Thermistors_Data.ThermistorValues[i][j]=68325/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    			else if(Thermistors_Data.ThermistorValues[i][j]>=1881)
	    			{
	    			    				//40 35
	    				Thermistors_Data.ThermistorValues[i][j]=65835/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    			else
	    			{
	    			    				//35 30
	    				Thermistors_Data.ThermistorValues[i][j]=61410/Thermistors_Data.ThermistorValues[i][j];
	    			}
	    				//
	    		}

	    	}

}


void checkTemperaturi()
{
	for(int i=0;i<THERMISTOR_BANKS;i++)
	    	{
	    		for(int j=0;j<THERMISTORS_PER_BANK;j++)
	    		{
	    			if(Thermistors_Data.ThermistorValues[i][j]>TEMP_MAX)
	    				icBaterie.flag=1;
	    		}
	    	}
}

void getAllTemps()
{
	for(int i = 0; i < THERMISTOR_BANKS; i++){
	        	GetTemp((uint16)i);
	        }

}

#ifdef __cplusplus
}
#endif

/** @} */
