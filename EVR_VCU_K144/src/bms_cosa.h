/*
 * bms_cosa.h
 *
 *  Created on: 29 apr. 2025
 *      Author: cosaa
 */

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



#endif /* BMS_COSA_H_ */
