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

uint8 ref0[3] = {1, 2, 3};
uint8 ref1[2] = {0, 4};

SevenSegmentGroup grupuri[2] = {
		{ref0, 3},
		{ref1, 2}
};
SevenSegmentDriver driver = {0, 0, grupuri, 2};



uint8 SetareDisplayDefault[2] = {0x01, 0x0f};
uint8 NormalMode[2] = {0x0c, 0x81}; // comanda normal mode
uint8 LuminozitateGlobala[2] = {0x0a, 0x0f}; // comanda luminozitate globala
uint8 NrPiniFolositi[2] = {0x0b, 0x03}; // cati pini de la dig0 pana la dig7 [ex: 0x00 - dig0 | 0x03 - dig0 -> dig3]
//uint8 SchimbareRegistruFeature[2] = {0x0e, 0x00};
uint8 Decodificator[2] = {0x09, 0xff}; // pana la ce pin folosim decodificare pe digits
uint8 Shutdown[2] = {0x0c, 0x00};


I2c_RequestType afisarenimic = {0, false, false, false, false, 2, I2C_SEND_DATA, SetareDisplayDefault};
I2c_RequestType setpins = {0, false, false, false, false, 2, I2C_SEND_DATA, NrPiniFolositi};
I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, Decodificator};
I2c_RequestType normalmode = {0, false, false, false, false, 2, I2C_SEND_DATA, NormalMode};
I2c_RequestType luminozitate = {0, false, false, false, false, 2, I2C_SEND_DATA, LuminozitateGlobala};
//I2c_RequestType feature = {0, false, false, false, false, 2, I2C_SEND_DATA, SchimbareRegistruFeature};
I2c_RequestType shutdown = {0, false, false, false, false, 2, I2C_SEND_DATA, Shutdown};

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

void SevenSegmentInit(void){
	I2c_SyncTransmit(driver.I2c_used_channel, &shutdown);
	I2c_SyncTransmit(driver.I2c_used_channel, &normalmode);
	I2c_SyncTransmit(driver.I2c_used_channel, &luminozitate);
	//I2c_SyncTransmit(driver.I2c_used_channel, &feature);

	for(int i = 0; i <= 7; i++){
		I2c_SyncTransmit(driver.I2c_used_channel, &afisarenimic);
		SetareDisplayDefault[0]++;
	}

	I2c_SyncTransmit(driver.I2c_used_channel, &setpins);
	I2c_SyncTransmit(driver.I2c_used_channel, &decodificator);
}

void SevenSegmentDisplayDecimalValue(uint8 SevenSegmentGroupIndex, uint8 DecimalValue, uint8 PrecisionFloatPoint){
	int aux;
	for(int i = 0; i < driver.group[SevenSegmentGroupIndex].nr_elemente; i++){
		if(i == PrecisionFloatPoint && PrecisionFloatPoint != 0)
			aux = DecimalValue % 10 + 128;
		else
			aux = DecimalValue % 10;

		uint8 AfisareDigit[2] = {driver.group[SevenSegmentGroupIndex].elemente[i], aux};
		I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
		if(DecimalValue == 0)
			break;
		else
			DecimalValue /= 10;
		I2c_SyncTransmit(driver.I2c_used_channel, &digit);
	}
}

void SevenSegmentSetGlobalBrightness(uint8 BrightnessPercent){
	if(BrightnessPercent > 100) BrightnessPercent = 100;
	LuminozitateGlobala[1] = (BrightnessPercent * 4) / 25;
	I2c_SyncTransmit(driver.I2c_used_channel, &luminozitate);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



#ifdef __cplusplus
}
#endif

/** @} */

