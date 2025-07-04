/*
 * bms_cosa.c
 *
 *  Created on: 29 apr. 2025
 *      Author: cosaa
 */
#include "bms_cosa.h"
#include "CDD_I2c.h"
#include "Dio.h"
#include "Icu.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "Spi.h"
#include "bms.h"
#include "uart_datasend.h"
#include "uart_error_handling.h"
#include "CDD_Uart.h"
#include "7-segment-display.h"
#include "bms.h"

extern uint8 buffTrimitere[64];
extern uint8 buffPrimire[64];
extern volatile int delei;
extern struct biemese icBaterie;
extern uint8 buffer[10];

extern uint8 pachete[6];

extern int numberOfSunturi;
extern int numberOfMonitoare;
extern int numberOfDevices;
extern uint16 dpec;

void BmsInit(void)
{
	buffTrimitere[0]=0;
	buffTrimitere[1]=0x27;
	transmisieCMD(); //SRST
}

void BmsTest(void)
{
    do
    {
        buffTrimitere[0]=0;
        buffTrimitere[1]=0x2C;
        transmisie(); //read RDSID
        //aprinde LED
    }
    while(buffPrimire[4]!=255);

}

/*
int BmsGetPackCurrent(void)
{
	buffTrimitere[0]=0;
	buffTrimitere[1]=0x44; //0x0C
	transmisieRD160();     //RDALLI

	icBaterie.packCurrent=((buffPrimire[6]<<16)+(buffPrimire[5]<<8)+(buffPrimire[4]))*20;

	return icBaterie.packCurrent;
}

int BmsGetPackVoltage(void)
{
	buffTrimitere[0]=0;
	buffTrimitere[1]=0x18;
	transmisieRD160();     //RDALLA
	//curent2=(buffPrimire[6]<<16)+(buffPrimire[5]<<8)+(buffPrimire[4]);
	//i2=5*curent2;//teoretic s-ar imparti la 4
	icBaterie.packVoltage=((buffPrimire[12]<<16)+(buffPrimire[11]<<8)+(buffPrimire[10]))*10;

	return icBaterie.packVoltage;
}
*/


void transmisie(void)
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 24U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}

void populeazaCMD(char MSB,char LSB)
{
	buffTrimitere[0] = MSB;
	buffTrimitere[1] = LSB;
}



void transmisieCMD(void)
{
	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	buffTrimitere[2] = pec >> 8;
	buffTrimitere[3] = pec % 256;
	#if 1
			Port_SetPinMode(9, PORT_MUX_AS_GPIO);
			Dio_WriteChannel(37, 0);
			delei = 30;
			while(delei){
				delei--;
			}
			Dio_WriteChannel(37, 1);
			Port_ResetPinMode(9);
			delei = 3000;
			while(delei){
				delei--;
			}
	#endif
			//comanda fara pec
			//TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
			//comanda cu pec


/*AICI*/    Spi_SetupEB(0u, buffTrimitere, buffPrimire, 64u);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}

void transmisieWR48(void)
{
	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	buffTrimitere[2] = pec >> 8;
	buffTrimitere[3] = pec % 256;
	#if 1
			Port_SetPinMode(9, PORT_MUX_AS_GPIO);
			Dio_WriteChannel(37, 0);
			delei = 30;
			while(delei){
				delei--;
			}
			Dio_WriteChannel(37, 1);
			Port_ResetPinMode(9);
			delei = 3000;
			while(delei){
				delei--;
			}
	#endif
			//comanda fara pec
			//TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
			//comanda cu pec


/*AICI*/    Spi_SetupEB(0u, buffTrimitere, buffPrimire, 4+8*NUMARUL_DE_SUNTURI+8*NUMARUL_DE_MONITOARE);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}


void flushTX()
{
	for(int i=0;i<64;i++)
	{
		buffTrimitere[i]=0;
		buffPrimire[i]=0;
	}
}

void SRST()
{
    populeazaCMD(0x00, 0x27);
    transmisieCMD(); //SRST
}

void RDSID()
{
    populeazaCMD(0x00, 0x2C);
    transmisieCMD(); //read RDSID
}

void RDCFGA()
{
    populeazaCMD(0x00,0x02);
    transmisieCMD();     //RDCFGA
}

void parametriiADC()
{
    populeazaCMD(0x00, 0x01);
    for(int i=0;i<NUMARUL_DE_SUNTURI;i++)
    {
        buffTrimitere[4+8*i]=0x0; //default
        buffTrimitere[5+8*i]=0; //CFGAR1
        buffTrimitere[6+8*i]=0; //CFGAR2
        buffTrimitere[7+8*i]=0x5F; //porneste GPIO
        buffTrimitere[8+8*i]=0x0;
        buffTrimitere[9+8*i]=0x10;
        dpec = pec10_calc(false,6U, buffTrimitere+4+8*i);
        buffTrimitere[10+8*i] = dpec >> 8;
        buffTrimitere[11+8*i] = dpec % 256;
    }

    for(int i=0;i<NUMARUL_DE_MONITOARE;i++)
    {
        buffTrimitere[4+8*NUMARUL_DE_SUNTURI+8*i]=0x81; //default
        buffTrimitere[5+8*NUMARUL_DE_SUNTURI+8*i]=0; //CFGAR1
        buffTrimitere[6+8*NUMARUL_DE_SUNTURI+8*i]=0; //CFGAR2
        buffTrimitere[7+8*NUMARUL_DE_SUNTURI+8*i]=0xFF; //porneste GPIO
        buffTrimitere[8+8*NUMARUL_DE_SUNTURI+8*i]=0x03;
        buffTrimitere[9+8*NUMARUL_DE_SUNTURI+8*i]=0x10;
        dpec = pec10_calc(false,6U, buffTrimitere+4+8*NUMARUL_DE_SUNTURI+8*i);
        buffTrimitere[10+8*NUMARUL_DE_SUNTURI+8*i] = dpec >> 8;
        buffTrimitere[11+8*NUMARUL_DE_SUNTURI+8*i] = dpec % 256;
    }
    transmisieWR48();     //WRCFGA
}

void CLRFLG()
{
    populeazaCMD(0x17,0x07);
    transmisieCMD(); //CLRFLG
}

void ADCV()
{
    populeazaCMD(0x03, 0xE0);
    transmisieCMD(); //ADCV
}

void readBieMieSe()
{
	volatile int delayul=1000000;
    for (int i = 0; i <= 3; i++) {
        delayul = MARELE_DELAY;
        while (delayul--) {
            // wait
        }

        ADCV();

        populeazaCMD(0,pachete[i]);
        transmisieCMD();

        icBaterie.cellVoltage[0+i*3]=15 * (buffPrimire[5] * 256 + buffPrimire[4]) + 150000;
        icBaterie.cellVoltage[1+i*3]=15 * (buffPrimire[7] * 256 + buffPrimire[6]) + 150000;
        icBaterie.cellVoltage[2+i*3]=15 * (buffPrimire[9] * 256 + buffPrimire[8]) + 150000;

        if (i == 0) {
            icBaterie.packCurrent = ((buffPrimire[17] << 16) + (buffPrimire[16] << 8) + (buffPrimire[15]))*5;

        }
        else if (i == 1) {
        	icBaterie.packVoltage = (buffPrimire[14] << 16) | (buffPrimire[13] << 8) | buffPrimire[12];
            if (icBaterie.packVoltage & 0x800000) {
            	icBaterie.packVoltage |= 0xFF000000;  // Set upper 8 bits to 1
            } else {
            	icBaterie.packVoltage &= 0x00FFFFFF;  // Clear upper 8 bits
            }

        }

    }
}
