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
#define MARELE_DELAY 10000
//30000 mergea binex
#define DELAY_COMENZI 10000
//300000 initial

#define CURENT_MAX 2000
//in MILIAMPERI te rog
#define UNDERVOLTAGE_CELL 1000
//in MILIVOLTI te implor
#define OVERVOLTAGE_CELL 3000
#define CURENT_STUPID 1000000
#define TENSIUNE_STUPID 10000

struct biemese
{
	int packCurrent;
	int packVoltage;
	int cellVoltage[BATTERY_CELLS];
	//Spi_ChannelType bmsSpiChannel;
};

void BmsInit(void);
void BmsTest(void);
void parametriiADC(void);
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
void CLRFLG(void);
void ADCV(void);
void readBieMieSe(void);
void sendAllUart(void);
void sendAMS(void);
void sendErori(void);

int getCelula(int index);
int getCurent(void);
int getVoltagePachet(void);
int CFGAok(void);
void bmsInit(void);
void sendEroareUnitate(int index);
int CRCok(uint8 *pointer);


#endif /* BMS_COSA_H_ */
