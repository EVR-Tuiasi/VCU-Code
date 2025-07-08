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

// -- Definire Grupuri de segmente si ce segmente se afla in ele

uint8 ref0[2] = {1, 2};
uint8 ref1[3] = {3, 4, 5};
uint8 ref2[3] = {6, 7, 8};

// -- Definite Structura de Grupuri, ce are buffer ul de Segmente si cate elemente se afla in fiecare grup

SevenSegmentGroup grupuri[3] = {
		{ref0, 2},
		{ref1, 3},
		{ref2, 3}
};
SevenSegmentDriver SevenSegmentDriverInstance = {0, 0, grupuri, 3}; // -- {Canal I2C folosit, Adresa Slave, Structura de grupuri de segmente, numarul de grupuri de segmente}


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
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &shutdown);

	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &luminozitate); // -- Seteaza Luminozitatea Globala la 7 Segment Display-uri

	SevSegInitBuf[0] = 0x0e, SevSegInitBuf[1] = 0x00; // -- Schimba Feature Register pentru modul de decodificare al 7 Segment Display
	I2c_RequestType feature = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &feature);

	SevSegInitBuf[0] = 0x01, SevSegInitBuf[1] = 0x0f;
	I2c_RequestType afisarenimic = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	for(int i = 0; i <= 7; i++){
		I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &afisarenimic); // -- Seteaza ca toate Segmentele de pe display sa fie stinse
		SevSegInitBuf[0]++;
	}

	SevSegInitBuf[0] = 0x0b, SevSegInitBuf[1] = 0x07; // -- Seteaza cati pini folosim de la dig0 pana la dig7 [ex: 0x00 - dig0 | 0x03 - dig0 -> dig3]
	I2c_RequestType setpins = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &setpins);

	SevSegInitBuf[0] = 0x09, SevSegInitBuf[1] = 0xff; // -- Seteaza pana la ce pin folosim decodificare pe digits [ex: 0x03 - 00000011 - Decodifica pe dig0 si dig1, ne luam dupa pozitia bitilor de la LSB la MSB]
	I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &decodificator);

	SevSegInitBuf[0] = 0x0c, SevSegInitBuf[1] = 0x81; // -- Seteaza Normal Mode fara modificari la Feature Register
	I2c_RequestType normalmode = {0, false, false, false, false, 2, I2C_SEND_DATA, SevSegInitBuf};
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &normalmode);
}

void SevenSegmentTest(void){
	uint16 rpm = 0, tensiune = 0, temperatura = 0, procentaj = 0, viteza;
	volatile uint32 delei;
	while(1){
		delei = 2000000;
		while(delei--);

		tensiune++;
		rpm+=5;
		temperatura++;
		tensiune %= 150;
		rpm %= 6000;
		temperatura %= 99;
        //calcul procentaj baterie
        if(tensiune < 60U){
            procentaj = 0;
        }
        else if(tensiune > 100){
        	procentaj = 1000U;//procentaj calculat cu o virgula
        }
        else{
        	procentaj = (uint16)(tensiune - 60U) * 25U;//procentaj calculat cu o virgula
        }
        if(procentaj < 1000U){
            SevenSegmentDisplayDecimalValue(2, procentaj, 1);
        }
        else{
            SevenSegmentDisplayDecimalValue(2, procentaj/10U, 0);
        }
        SevenSegmentDisplayDecimalValue(0, temperatura, 0);
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
	}
}

void SevSegGrTest(uint8 GroupIndex){
	uint8 caz = 0; // -- Aceasta variabila este folosita pentru a face testele, fiind verificata intr un switch


	while(1){
		volatile int delay = 8000000;
		uint8 luminozitateTemp = 0;
		if(caz <= 7){ // -- Acest If verifica daca suntem in range ul de cazuri pentru test
			while(delay != 0) // -- Acest While face un delay de o secunda [aproimare generoasa]
				delay--;
		}

		switch(caz){ // -- Aici incep testele in Switch dupa ce trecem de While
			case 0:
				SevenSegmentDisplayDecimalValue(GroupIndex, 0, 0); caz++; // -- Afisam pe grupul g de segmente [ ][ ][ ][0]
				break;
			case 1:
				SevenSegmentDisplayDecimalValue(GroupIndex, 3331, 0); caz++; // -- Afisam pe grupul g de segmente [3][3][3][1]
				break;
			case 2:
				SevenSegmentDisplayDecimalValue(GroupIndex, 12, 1); caz++; // -- Afisam pe grupul g de segmente [ ][ ][1.][2]
				break;
			case 3:
				SevenSegmentDisplayDecimalValue(GroupIndex, 1, 1); caz++; // -- Afisam pe grupul g de segmente [ ][ ][0.][1]
				break;
			case 4:
				SevenSegmentDisplayDecimalValue(GroupIndex, -12, 0); caz++; // -- Afisam pe grupul g de segmente [ ][-][1][2]
				break;
			case 5:
				SevenSegmentDisplayDecimalValue(GroupIndex, -123, 2); caz++; // -- Afisam pe grupul g de segmente [ ][-][0.][1]
				break;
			case 6:
				SevenSegmentDisplayDecimalValue(GroupIndex, -12, 2); caz++; // -- Afisam pe grupul g de segmente [-][0.][1][2]
				break;
			case 7:
				SevenSegmentDisplayDecimalValue(GroupIndex, -1, 1); caz++; // -- Afisam pe grupul g de segmente [-][1][2][3]
				break;
			case 8:
				while(1){
					volatile int delaytemp = 100000;
					while(delaytemp != 0)
						delaytemp--;

					SevenSegmentSetGlobalBrightness(luminozitateTemp);
					if(luminozitateTemp == 100)
						break;
					luminozitateTemp++;
				}


			default:
				caz = 0; // -- Daca trecem de ultimul caz, resetam ordinea si incepem de la cazul 0
				break;
		}
	}
}

void SevenSegmentDisplayDecimalValue(uint8 SevenSegmentGroupIndex, sint16 DecimalValue, uint8 PrecisionFloatPoint){
	if(SevenSegmentGroupIndex > SevenSegmentDriverInstance.SevenSegmentGroup_elements_count){ // -- Daca grupul precizat nu exista, afisam codul de eraore si iesim din functie
		; // TODO de inserat apel la functia de eroare
	}
	else if((DecimalValue == 0) && (PrecisionFloatPoint == 0)){ // -- Daca valoare pe care o vrem afisata este 0, fara virgula, afisam doar 0 pe primul segment din dreapta apoi restul goale

		uint8 AfisareDigit[2] = {SevenSegmentDriverInstance.group[SevenSegmentGroupIndex].elemente[0], DecimalValue};
		I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
		I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &digit);

		for(int i = 1; i < SevenSegmentDriverInstance.group[SevenSegmentGroupIndex].nr_elemente; i++){
			AfisareDigit[0] = SevenSegmentDriverInstance.group[SevenSegmentGroupIndex].elemente[i];
			AfisareDigit[1] = 15;
			I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
			I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &digit);
		}

		AfisareDigit[0] = 0x09, AfisareDigit[1] = 0xff;
		I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &digit);
	}
	else { // -- Aici afisam valoarea daca aceasta este diferita de 0 [pozitiva/negativa]
		int aux;
		bool isNegative = false, isPositive = true;
		uint8 tempBuf[2] = {0x09, 0x00};

		if(DecimalValue < 0){ // -- Aici verificam daca numarul este pozitiv sau negativ
			isNegative = true;
			isPositive = false;
			DecimalValue *= -1;
		}

		for(int i = 0; i < SevenSegmentDriverInstance.group[SevenSegmentGroupIndex].nr_elemente; i++){
			if(i == PrecisionFloatPoint && PrecisionFloatPoint != 0) // -- Aici setam virgula daca numarul este cu virgula si se ajunge la pozitia segmentului unde ar trebui afisata
				aux = DecimalValue % 10 + 128;
			else
				aux = DecimalValue % 10;

			uint8 AfisareDigit[2] = {SevenSegmentDriverInstance.group[SevenSegmentGroupIndex].elemente[i], aux}; // -- Definim buffer ul de afisare la fiecare cifra

			if((DecimalValue == 0) && (PrecisionFloatPoint >= i)){ // -- Aici verificam daca avem numarul de afisat zero dar inca nu am ajuns la virgula
				if(PrecisionFloatPoint > i) // -- Daca nu ajungem la pozitia virgulei, scriem 0 fara virgula
					AfisareDigit[1] = 0;
				else if(PrecisionFloatPoint == i) // -- Daca ajungem la pozitia virgulei, scriem 0 cu virgula
					AfisareDigit[1] = 128;
			}
			else if(DecimalValue == 0){ // Daca ajungem la capatul numarului, verificam ce facem daca numarul este pozitiv sau negativ
				if(isNegative){ // -- Daca este negativ setam ca segmentul unde vine "-" sa nu mai aiba decodificare, si setam ca valoare afisata sa fie 0000 0001 [informatii mai detaliate la afisarea segmentelor in datasheet]
					tempBuf[1] = ~(1<<(i)); // -- Selectam ca pozitia unde vine "-" sa nu aiba decodificare [ex: daca vrem digit 2 sa aiba minus vom face 1<<(1), pentru a avea 0000 0010, iar apoi inversam prin operatorul "~" pentru a seta decodificare pe toate segmentele inafara de acel unde afisam "-"]
					I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, tempBuf};
					I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &decodificator);

					AfisareDigit[1] = 1; // -- Setam ca doar segmentul G sa fie aprins, ca sa avem afisat "-"
					isNegative = false;
				} else if(isPositive){ // -- Daca este pozitiv, setam decodificarea sa fie pe toate segmentele, asigurandu ne ca nu ramanem cu segmente fara decodificare, in caz ca anterior am afisat un numar negativ
					tempBuf[1] = 0xff;
					I2c_RequestType decodificator = {0, false, false, false, false, 2, I2C_SEND_DATA, tempBuf};
					I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &decodificator);

					AfisareDigit[1] = 15; // -- Setam ca segmentul sa nu afiseze nimic [mai multe informatii la ce afiseaza in datasheet]
				}
				else
					AfisareDigit[1] = 15; // -- Setam ca segmentul sa nu afiseze nimic [mai multe informatii la ce afiseaza in datasheet]
			}
			else
				DecimalValue /= 10; // -- Dupa ce terminam de setat ce afisam, divizam cu 10 ca sa ajungem la urmatoarea cifra afisata [ex: numar de afisat 123, divizam cu 10 ca sa ajungem la valoare 12]

			I2c_RequestType digit = {0, false, false, false, false, 2, I2C_SEND_DATA, AfisareDigit};
			I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &digit);  // -- Trimitem la SevenSegmentDriverInstance valoarea pe care vrem sa o afisam ["-" sau o cifra, cu sau fara virgula]
		}
	}
}

void SevenSegmentSetGlobalBrightness(uint8 BrightnessPercent){
	if(BrightnessPercent > 100) // -- limitam ca procentul de luminozitate sa nu fie peste 100%
		BrightnessPercent = 100;

	LuminozitateGlobala[1] = (uint8)(((uint16)BrightnessPercent * (uint16)3) / (uint16)20); // -- transformam procentul intr o valoare din int. 0 - 16
	I2c_SyncTransmit(SevenSegmentDriverInstance.I2c_used_channel, &luminozitate);
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



#ifdef __cplusplus
}
#endif

/** @} */

