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
#include "thermistor_mux.h"
///aici normal

extern uint8 buffTrimitere[64];
extern uint8 buffPrimire[64];
extern volatile int delei;
struct biemese icBaterie;
extern uint8 buffer[10];

extern uint8 pachete[6];
extern uint8 pacheteS[6];

extern int numberOfSunturi;
extern int numberOfMonitoare;
extern int numberOfDevices;
extern uint16 dpec;
extern Thermistors Thermistors_Data;

volatile int tensiuneMILIvolti1,tensiuneMILIvolti2, tensiuneMILIvolti3;

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

void RDCFGB()
{
    populeazaCMD(0x00,0x26);
    transmisieCMD();     //RDCFGB
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

        	/*if(icBaterie.cellVoltage[j*12+0+i*3]>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+0+i*3]=0;
        		//sendEroareUnitate(j);
        	}
        	if(icBaterie.cellVoltage[j*12+1+i*3]>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+1+i*3]=0;
        		//sendEroareUnitate(j);
        	}
        	if(icBaterie.cellVoltage[j*12+2+i*3]>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+2+i*3]=0;
        		//sendEroareUnitate(j);
        	}*/
		}


        if (i == 0) {
            icBaterie.packCurrent = ((buffPrimire[5+4+8*NUMARUL_DE_MONITOARE] << 16) + (buffPrimire[4+4+8*NUMARUL_DE_MONITOARE] << 8) + (buffPrimire[3+4+8*NUMARUL_DE_MONITOARE]))*5;
            if(icBaterie.packCurrent>CURENT_STUPID || icBaterie.packCurrent < 0)
            {
            	icBaterie.packCurrent=0;
            	//sendEroareUnitate(NUMARUL_DE_MONITOARE);
            }


        }
        else if (i == 1) {
        	icBaterie.packVoltage = (buffPrimire[2+4+8*NUMARUL_DE_MONITOARE] << 16) | (buffPrimire[1+4+8*NUMARUL_DE_MONITOARE] << 8) | buffPrimire[4+8*NUMARUL_DE_MONITOARE];
            if (icBaterie.packVoltage & 0x800000) {
            	icBaterie.packVoltage |= 0xFF000000;  // Set upper 8 bits to 1
            } else {
            	icBaterie.packVoltage &= 0x00FFFFFF;  // Clear upper 8 bits
            }

            if(icBaterie.packVoltage>TENSIUNE_STUPID || icBaterie.packVoltage<10)
            {
            	icBaterie.packVoltage=0;
            	icBaterie.packCurrent=0;
            	//sendEroareUnitate(NUMARUL_DE_MONITOARE);
            }


        }

    }
}

void readBieMieSeOW()
{
	volatile int delayul;
    for (int i = 0; i <= 3; i++) {
        delayul = MARELE_DELAY;
        while (delayul--) {
            // wait
        }

        populeazaCMD(0x01, 0x6A); //69
        transmisieCMD(); //ADSV OW par


        populeazaCMD(0,pacheteS[i]);
        transmisieCMD();



        for(int j=0;j<NUMARUL_DE_MONITOARE;j++)
        {
        	tensiuneMILIvolti1=15 * (buffPrimire[5+8*j] * 256 + buffPrimire[4+8*j]) + 150000;
        	tensiuneMILIvolti2=15 * (buffPrimire[7+8*j] * 256 + buffPrimire[6+8*j]) + 150000;
        	tensiuneMILIvolti3=15 * (buffPrimire[9+8*j] * 256 + buffPrimire[8+8*j]) + 150000;

        	if(tensiuneMILIvolti1>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+0+i*3]=0;
        		sendEroareUnitate(j*12+0+i*3);
        		icBaterie.flag=true;
        	}
        	if(tensiuneMILIvolti2>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+1+i*3]=0;
        		sendEroareUnitate(j*12+1+i*3);
        		icBaterie.flag=true;
        	}
        	if(tensiuneMILIvolti3>CELULA_STUPID)
        	{
        		icBaterie.cellVoltage[j*12+2+i*3]=0;
        		sendEroareUnitate(j*12+2+i*3);
        		icBaterie.flag=true;
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
    buffer[5] = CRC_calculate(6);
    Uart_SyncSend(0, buffer, 6, 10000000);

    buffer[0] = 12;
    buffer[1] = (icBaterie.packVoltage >> 24) % 256;
    buffer[2] = (icBaterie.packVoltage >> 16) % 256;
    buffer[3] = (icBaterie.packVoltage >> 8)  % 256;
    buffer[4] = icBaterie.packVoltage % 256;
    buffer[5] = CRC_calculate(6);
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
		buffer[7] = CRC_calculate(8);
		Uart_SyncSend(0, buffer, 8, 10000000);
    }
    /*
	for(int i=0;i<THERMISTOR_BANKS;i++)
	{
		for(int j=0;j<THERMISTORS_PER_BANK;j++)
		{
			buffer[0] = 10;
			buffer[1] = 0;
			buffer[2] = i*8+j;
			buffer[3] = 0;
			buffer[4] = 0;
			buffer[5] = (Thermistors_Data.ThermistorValues[i][j] >> 8)  % 256;
			buffer[6] = Thermistors_Data.ThermistorValues[i][j] % 256;
			buffer[7] = CRC_calculate(8);
			Uart_SyncSend(0, buffer, 8, 10000000);


		}
	}*/
}

void sendAMS(void)
{
	if(icBaterie.packCurrent > CURENT_MAX)
	{
		icBaterie.flag=true; //cevaEroare
		icBaterie.stateSHUNT=icBaterie.stateSHUNT|32;
	}
	if(icBaterie.packVoltage > TENSIUNE_MAX)
		{
			icBaterie.flag=true; //cevaEroare
			icBaterie.stateSHUNT=icBaterie.stateSHUNT|4;
		}
	if(icBaterie.packVoltage < TENSIUNE_MIN)
			{
				icBaterie.flag=true; //cevaEroare
				icBaterie.stateSHUNT=icBaterie.stateSHUNT|2;
			}




}



void sendOW()
{
	for(int i=0;i<BATTERY_CELLS;i++)
		{
			if(icBaterie.cellVoltage[i]<UNDERVOLTAGE_CELL)
			{
				icBaterie.flag=true; //cevaEroare
				icBaterie.stateBMS[i/12]=icBaterie.stateBMS[i/12]|8;
			}
			else if(icBaterie.cellVoltage[i]>OVERVOLTAGE_CELL)
			{
				icBaterie.flag=true; //cevaEroare
				icBaterie.stateBMS[i/12]=icBaterie.stateBMS[i/12]|4;
			}
		}
}




void sendErori(void)
{
	buffer[0]=100;
	buffer[1]=0x07;
	buffer[2]=icBaterie.stateSHUNT;
	buffer[3]=CRC_calculate(4);
	Uart_SyncSend(0, buffer, 4, 10000000);

	buffer[0]=100;
	buffer[1]=0x08;
	buffer[2]=icBaterie.stateBMS[0];
	buffer[3]=CRC_calculate(4);
	Uart_SyncSend(0, buffer, 4, 10000000);

	buffer[0]=100;
	buffer[1]=0x09;
	buffer[2]=icBaterie.stateBMS[1];
	buffer[3]=CRC_calculate(4);
	Uart_SyncSend(0, buffer, 4, 10000000);


}

void clearStates()
{
	for(int i=0;i<NUMARUL_DE_MONITOARE;i++)
		icBaterie.stateBMS[i]=0;
	icBaterie.stateSHUNT=0;
}

uint8 CRC_calculate(uint8 length){
	uint8 crc=0, message[length];
	uint16 divisor = 0x8D, dividend;
	int i, j;

	for(i=0; i<length-1; i++)
	{
		message[i] = buffer[i];
	}

	message[length-1]=0;

	dividend = (message[0] << 8) | message[1];
	for(j=15; j>=8; j--)
		if(dividend & (1 << j))
			dividend ^= divisor << (j-8);

	for(i=2; i<length; i++)
		{
			dividend = (dividend << 8) | message[i];

			for(j=15; j>=8; j--)
				if(dividend & (1 << j))
					dividend ^= divisor << (j-8);
		}

	crc = (dividend % 256);

	return crc;
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
        if(buffPrimire[4 + 8*i] != 0x00)        // default
            return false;
        if(buffPrimire[5 + 8*i] != 0x00)        // CFGAR1
            return false;
        if(buffPrimire[6 + 8*i] != 0x00)        // CFGAR2
            return false;
        if(buffPrimire[7 + 8*i] != 0x5F)        // porneste GPIO
            return false;
        if(buffPrimire[8 + 8*i] != 0x00)
            return false;
        if(buffPrimire[9 + 8*i] != 0x10)
            return false;
    }

    for(int i = 0; i < NUMARUL_DE_MONITOARE; i++)
    {
        offset = 4 + 8*NUMARUL_DE_SUNTURI + 8*i;
        if(buffPrimire[offset + 0] != 0x81)      // default
            return false;
        if(buffPrimire[offset + 1] != 0x00)      // CFGAR1
            return false;
        if(buffPrimire[offset + 2] != 0x00)      // CFGAR2
            return false;
        if(buffPrimire[offset + 3] != 0xFF)      // porneste GPIO
            return false;
        if(buffPrimire[offset + 4] != 0x03)
            return false;
        if(buffPrimire[offset + 5] != 0x10)
            return false;
    }

    return true;
}

void bmsInit(void)
{

    parametriiADC(); //bmsINIT
    ADCV();
    flushTX();
    clearStates();
}

void sendEroareUnitate(int index)
//trimite eroare ca modulul index este bulit
{
	buffer[0]=100;
	buffer[1]=0xF8;
	buffer[2]=index+1;
	buffer[3]=CRC_calculate(4);
	Uart_SyncSend(0, buffer, 4, 10000000);

}

int CRCok(uint8 *pointer) //nu merge
{
	uint16 peculCalculat=pec10_calc(true,6U, pointer);
	uint16 pecPrimit= (pointer[7]<<8)|pointer[6];
	return peculCalculat==pecPrimit;
}

void readShuntOW()
{
	volatile int delayul;
	delayul = MARELE_DELAY;
    while (delayul--) {}

    populeazaCMD(0x01, 0xFA); //69
    transmisieCMD(); //ADSV OW par


}
