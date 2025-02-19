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

uint8 ref0[4] = {1, 2, 3, 4};
uint8 ref1[2] = {3, 4};

SevenSegmentGroup grupuri[2] = {
		{ref0, 4},
		{ref1, 2}
};
SevenSegmentDriver driver = {0, 0, grupuri, 2};


uint8 LuminozitateGlobala[2] = {0x0a, 0x0f}; // -- Buffer Luminozitate Globala maxima

I2c_RequestType luminozitate = {0, false, false, false, false, 2, I2C_SEND_DATA, LuminozitateGlobala};


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
	uint8 SevSegInitBuf[2] = {0x00, 0x00}; // -- Buffer-ul din functia "SevenSegmentInit();"

	SevSegInitBuf[0] = 0x0c, SevSegInitBuf[1] = 0x00; // -- Seteaza modul Shutdown cu Reset Feature Register
	I2c_RequestType shutdown = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(driver.I2c_used_channel, &shutdown);

	I2c_SyncTransmit(driver.I2c_used_channel, &luminozitate); // -- Seteaza Luminozitatea Globala la 7 Segment Display-uri

	SevSegInitBuf[0] = 0x0e, SevSegInitBuf[1] = 0x00; // -- Schimba Feature Register pentru modul de decodificare al 7 Segment Display
	I2c_RequestType feature = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(driver.I2c_used_channel, &feature);

	SevSegInitBuf[0] = 0x01, SevSegInitBuf[1] = 0x0f;
	I2c_RequestType afisarenimic = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	for(int i = 0; i <= 7; i++){
		I2c_SyncTransmit(driver.I2c_used_channel, &afisarenimic); // -- Seteaza ca toate Segmentele de pe display sa fie stinse
		SevSegInitBuf[0]++;
	}

	SevSegInitBuf[0] = 0x0b, SevSegInitBuf[1] = 0x03; // -- Seteaza cati pini folosim de la dig0 pana la dig7 [ex: 0x00 - dig0 | 0x03 - dig0 -> dig3]
	I2c_RequestType setpins = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(driver.I2c_used_channel, &setpins);

	SevSegInitBuf[0] = 0x09, SevSegInitBuf[1] = 0xff; // -- Seteaza pana la ce pin folosim decodificare pe digits [ex: 0x03 - 00000011 - Decodifica pe dig0 si dig1, ne luam dupa pozitia bitilor de la LSB la MSB]
	I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(driver.I2c_used_channel, &decodificator);

	SevSegInitBuf[0] = 0x0c, SevSegInitBuf[1] = 0x81; // -- Seteaza Normal Mode fara modificari la Feature Register
	I2c_RequestType normalmode = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(driver.I2c_used_channel, &normalmode);
}

bool SevSegGroupVerification(uint8 SevenSegmentGroupIndex){
	if(SevenSegmentGroupIndex < driver.SevenSegmentGroup_elements_count)
		return true;
	else
		return false;
}

void SevSegGrTest(uint8 g){
	uint8 caz = 0;

	while(1){
		volatile int n = 8000000;
		if(caz <= 7){
			while(n != 0)
				n--;
		}

		switch(caz){
			case 0:
				SevenSegmentDisplayDecimalValue(g, 0, 0); caz++;
				break;
			case 1:
				SevenSegmentDisplayDecimalValue(g, 3331, 0); caz++;
				break;
			case 2:
				SevenSegmentDisplayDecimalValue(g, 12, 1); caz++;
				break;
			case 3:
				SevenSegmentDisplayDecimalValue(g, 1, 1); caz++;
				break;
			case 4:
				SevenSegmentDisplayDecimalValue(g, -12, 0); caz++;
				break;
			case 5:
				SevenSegmentDisplayDecimalValue(g, -1, 1); caz++;
				break;
			case 6:
				SevenSegmentDisplayDecimalValue(g, -12, 2); caz++;
				break;
			case 7:
				SevenSegmentDisplayDecimalValue(g, -123, 2); caz++;
				break;

			default:
				caz = 0;
				break;
		}
	}
}

void SevenSegmentDisplayDecimalValue(uint8 SevenSegmentGroupIndex, sint16 DecimalValue, uint8 PrecisionFloatPoint){
	if(!SevSegGroupVerification(SevenSegmentGroupIndex)){
		; // TODO de inserat apel la functia de eroare
	}
	else if((DecimalValue == 0) && (PrecisionFloatPoint == 0)){
		uint8 AfisareDigit[2] = {driver.group[SevenSegmentGroupIndex].elemente[0], DecimalValue};
		I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
		I2c_SyncTransmit(driver.I2c_used_channel, &digit);

		for(int i = 1; i < driver.group[SevenSegmentGroupIndex].nr_elemente; i++){
			AfisareDigit[0] = driver.group[SevenSegmentGroupIndex].elemente[i];
			AfisareDigit[1] = 15;
			I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
			I2c_SyncTransmit(driver.I2c_used_channel, &digit);
		}

		AfisareDigit[0] = 0x09, AfisareDigit[1] = 0xff;
		I2c_SyncTransmit(driver.I2c_used_channel, &digit);
	}
	else {
		int aux;
		bool isNegative = false, isPositive = true;
		uint8 tempBuf[2] = {0x09, 0x00};

		if(DecimalValue < 0){
			isNegative = true;
			isPositive = false;
			DecimalValue *= -1;
		}

		for(int i = 0; i < driver.group[SevenSegmentGroupIndex].nr_elemente; i++){
			if(i == PrecisionFloatPoint && PrecisionFloatPoint != 0)
				aux = DecimalValue % 10 + 128;
			else
				aux = DecimalValue % 10;

			uint8 AfisareDigit[2] = {driver.group[SevenSegmentGroupIndex].elemente[i], aux};
			if((DecimalValue == 0) && (PrecisionFloatPoint >= i)){
				if(PrecisionFloatPoint > i)
					AfisareDigit[1] = 0;
				else if(PrecisionFloatPoint == i)
					AfisareDigit[1] = 128;
			}
			else if(DecimalValue == 0){
				if(isNegative){
					tempBuf[1] = ~(1<<(i));
					I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, tempBuf};
					I2c_SyncTransmit(driver.I2c_used_channel, &decodificator);

					AfisareDigit[1] = 1;
					isNegative = false;
				} else if(isPositive){
					tempBuf[1] = 0xff;
					I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, tempBuf};
					I2c_SyncTransmit(driver.I2c_used_channel, &decodificator);

					AfisareDigit[1] = 15;
				}
				else
					AfisareDigit[1] = 15;
			}
			else
				DecimalValue /= 10;

			I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
			I2c_SyncTransmit(driver.I2c_used_channel, &digit);
		}
	}
}

void SevenSegmentSetGlobalBrightness(uint8 BrightnessPercent){
	if(BrightnessPercent > 100)
		BrightnessPercent = 100;

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

