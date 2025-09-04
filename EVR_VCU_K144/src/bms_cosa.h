/*
 * bms_cosa.h
 *
 *  Created on: 29 apr. 2025
 *      Author: cosaa
 */

#include "Mcu.h"
#ifndef BMS_COSA_H_
#define BMS_COSA_H_
#define BATTERY_CELLS 24
#define BOARDS_NUMBER 5
#define NUMARUL_DE_MONITOARE 2
#define NUMARUL_DE_SUNTURI 1
#define CRC_DARIUS 0x8D
#define MARELE_DELAY 100000//4000
//30000 mergea binex
#define DELAY_COMENZI 100000//4000
//300000 initial

#define CURENT_MAX 600000 //600A
//in MILIAMPERI te rog
#define UNDERVOLTAGE_CELL 250000 //3.5V pentru moment
//in MILIVOLTI te implor
#define TENSIUNE_MAX 10100 //101V
#define TENSIUNE_MIN 6000 //60V
#define OVERVOLTAGE_CELL 423000 //4.23V pentru moment
#define CURENT_STUPID 1000000
#define TENSIUNE_STUPID 1000000
#define CELULA_STUPID 500000
//100k e 10v

struct biemese
{
	int packCurrent;
	int packVoltage;
	int cellVoltage[BATTERY_CELLS];
	bool flag;
	uint8 stateBMS[NUMARUL_DE_MONITOARE];
	uint8 stateSHUNT;
	//Spi_ChannelType bmsSpiChannel;
};

void BmsInit(void);
void BmsTest(void);
void parametriiADC(void);
void parametriiADCB(void);
int BmsGetPackCurrent(void);
int BmsGetPackVoltage(void);

void transmisie(void);

void transmisieCMD(void);
void transmisieRD48(void);


void populeazaCMD(char MSB,char LSB);

void flushTX(void);

void SRST(void);
void RDSID(void);
void RDCFGA(void);
void RDCFGB(void);
void CLRFLG(void);
void ADCV(void);
void readBieMieSe(void);
void readBieMieSeOW(void);
void sendAllUart(void);
void sendAMS(void);
void sendOW(void);
void sendErori(void);

int getCelula(int index);
int getCurent(void);
int getVoltagePachet(void);
int CFGAok(void);
void bmsInit(void);
void sendEroareUnitate(int index);
int CRCok(uint8 *pointer);
void clearStates(void);
void readShuntOW(void);

uint8 CRC_calculate(uint8 length);


#endif /* BMS_COSA_H_ */
