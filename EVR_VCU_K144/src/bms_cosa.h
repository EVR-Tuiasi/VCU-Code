/*
 * bms_cosa.h
 *
 *  Created on: 29 apr. 2025
 *      Author: cosaa
 */

#ifndef BMS_COSA_H_
#define BMS_COSA_H_
#define BATTERY_CELLS 600
#define BOARDS_NUMBER 5

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

void transmisieWR48(void);

void transmisieRD160(void);

void populeazaCMD(char MSB,char LSB);

void flushTX(void);

#endif /* BMS_COSA_H_ */
