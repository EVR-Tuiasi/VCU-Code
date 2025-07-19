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
///aici normal

extern uint8 buffTrimitere[64];
extern uint8 buffPrimire[64];
extern volatile int delei;
struct biemese icBaterie;
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
	    	delei = DELAY_COMENZI;
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
        for(int j=0;j<NUMARUL_DE_MONITOARE;j++)
        {
            icBaterie.cellVoltage[j*12+0+i*3]=15 * (buffPrimire[5+8*j] * 256 + buffPrimire[4+8*j]) + 150000;
            icBaterie.cellVoltage[j*12+1+i*3]=15 * (buffPrimire[7+8*j] * 256 + buffPrimire[6+8*j]) + 150000;
            icBaterie.cellVoltage[j*12+2+i*3]=15 * (buffPrimire[9+8*j] * 256 + buffPrimire[8+8*j]) + 150000;
        }


        if (i == 0) {
            icBaterie.packCurrent = ((buffPrimire[5+4+8*NUMARUL_DE_MONITOARE] << 16) + (buffPrimire[4+4+8*NUMARUL_DE_MONITOARE] << 8) + (buffPrimire[3+4+8*NUMARUL_DE_MONITOARE]))*5;

        }
        else if (i == 1) {
        	icBaterie.packVoltage = (buffPrimire[2+4+8*NUMARUL_DE_MONITOARE] << 16) | (buffPrimire[1+4+8*NUMARUL_DE_MONITOARE] << 8) | buffPrimire[4+8*NUMARUL_DE_MONITOARE];
            if (icBaterie.packVoltage & 0x800000) {
            	icBaterie.packVoltage |= 0xFF000000;  // Set upper 8 bits to 1
            } else {
            	icBaterie.packVoltage &= 0x00FFFFFF;  // Clear upper 8 bits
            }

        }

    }
}

void sendAllUart()
{
    buffer[0] = 13;
    buffer[1] = (icBaterie.packCurrent >> 24) % 256;
    buffer[2] = (icBaterie.packCurrent >> 16) % 256;
    buffer[3] = (icBaterie.packCurrent >> 8)  % 256;
    buffer[4] = icBaterie.packCurrent % 256;
    buffer[5] = CRC_DARIUS;
    Uart_SyncSend(0, buffer, 6, 10000000);

    buffer[0] = 12;
    buffer[1] = (icBaterie.packVoltage >> 24) % 256;
    buffer[2] = (icBaterie.packVoltage >> 16) % 256;
    buffer[3] = (icBaterie.packVoltage >> 8)  % 256;
    buffer[4] = icBaterie.packVoltage % 256;
    buffer[5] = CRC_DARIUS;
    Uart_SyncSend(0, buffer, 6, 10000000);


    for(int i=0;i<BATTERY_CELLS;i++)
    {
		buffer[0] = 11;
		buffer[1] = 0;
		buffer[2] = i;
		buffer[3] = (icBaterie.cellVoltage[i]>>24) % 256;
		buffer[4] = (icBaterie.cellVoltage[i] >> 16) % 256;
		buffer[5] = (icBaterie.cellVoltage[i] >> 8)  % 256;
		buffer[6] = icBaterie.cellVoltage[i] % 256;
		buffer[7] = CRC_DARIUS;
		Uart_SyncSend(0, buffer, 8, 10000000);
    }
}

void sendAMS(void)
{
	if(icBaterie.packCurrent > CURENT_MAX)
	{
		buffer[0] = 11; //cevaEroare
	}
}

void sendErori(void)
{
	for(int i=0;i<BATTERY_CELLS;i++)
	{
		if(icBaterie.cellVoltage[i]<UNDERVOLTAGE_CELL)
		{
			buffer[0] = 11; //cevaEroare
			buffer[0] = i;  //unde crapa
		}
		else if(icBaterie.cellVoltage[i]>OVERVOLTAGE_CELL)
		{
			buffer[0] = 11; //cevaEroare
			buffer[0] = i;  //unde crapa
		}
	}
}

int getCelula(int index) //returneaza tensiunea celulei X
{
	if(index<BATTERY_CELLS)
	{
		return icBaterie.cellVoltage[index];

	}
	return 0;
}

int getCurent(void)
{
	return icBaterie.packCurrent;
}

int getVoltagePachet(void)
{
	return icBaterie.packVoltage;
}

int CFGAok(void) // returneaza TRUE daca TOTI registrii din serie sunt conform configuratiei
{
    RDCFGA();
    int offset;
    for(int i = 0; i < NUMARUL_DE_SUNTURI; i++)
    {
        if(buffTrimitere[4 + 8*i] != 0x00)        // default
            return false;
        if(buffTrimitere[5 + 8*i] != 0x00)        // CFGAR1
            return false;
        if(buffTrimitere[6 + 8*i] != 0x00)        // CFGAR2
            return false;
        if(buffTrimitere[7 + 8*i] != 0x5F)        // porneste GPIO
            return false;
        if(buffTrimitere[8 + 8*i] != 0x00)
            return false;
        if(buffTrimitere[9 + 8*i] != 0x10)
            return false;
    }

    for(int i = 0; i < NUMARUL_DE_MONITOARE; i++)
    {
        offset = 4 + 8*NUMARUL_DE_SUNTURI + 8*i;
        if(buffTrimitere[offset + 0] != 0x81)      // default
            return false;
        if(buffTrimitere[offset + 1] != 0x00)      // CFGAR1
            return false;
        if(buffTrimitere[offset + 2] != 0x00)      // CFGAR2
            return false;
        if(buffTrimitere[offset + 3] != 0xFF)      // porneste GPIO
            return false;
        if(buffTrimitere[offset + 4] != 0x03)
            return false;
        if(buffTrimitere[offset + 5] != 0x10)
            return false;
    }

    return true;
}

void bmsInit(void)
{

    parametriiADC(); //bmsINIT
    ADCV();
    flushTX();
}

void sendEroareUnitate(int index)
//trimite eroare ca modulul index este bulit
{
	buffer[0]=index;
}
