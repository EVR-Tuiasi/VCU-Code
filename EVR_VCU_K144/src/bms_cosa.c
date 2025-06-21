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

extern int numberOfSunturi;
extern int numberOfMonitoare;
extern int numberOfDevices;

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

void parametriiADC(void)
{
    buffTrimitere[0]=0;
    buffTrimitere[1]=0x1;

    buffTrimitere[4]=0x81; //default
    buffTrimitere[5]=0; //CFGAR1
    buffTrimitere[6]=0; //CFGAR2
    buffTrimitere[7]=0xFF; //porneste GPIO
    buffTrimitere[8]=0x03;
    buffTrimitere[9]=0x10;
    transmisieWR48(); //WRCFGA




}

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



void transmisieCMD(void)
{


	    	for (int i=0;i<numberOfMonitoare;i++)
	    	{
	    		uint16 pec = Pec15_Calc(2U, buffTrimitere+i*4);
	    		buffTrimitere[2+i*4] = pec >> 8;
	    		buffTrimitere[3+i*4] = pec % 256;
	    	}

	    	for (int i=0;i<numberOfSunturi;i++)
	    	{
	    		uint16 pec = Pec15_Calc(2U, buffTrimitere+i*4+numberOfMonitoare*4);
	    		buffTrimitere[2+i*4+numberOfMonitoare*4] = pec >> 8;
	    		buffTrimitere[3+i*4+numberOfMonitoare*4] = pec % 256;
	    	}

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

void transmisieRD48(void)
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
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 12U);
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
	    	uint16 dpec = pec10_calc(false,6U, buffTrimitere+4);
	    	buffTrimitere[10] = dpec >> 8;
	    	buffTrimitere[11] = dpec % 256;

	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 12U);
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

void transmisieRD160(void)
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
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 32U);
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

