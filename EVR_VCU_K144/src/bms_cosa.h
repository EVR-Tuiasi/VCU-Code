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

void BmsInit();
void BmsTest();
void parametriiADC();
int BmsGetPackCurrent();

void transmisie();

void transmisieCMD();
void transmisieRD48();

void transmisieWR48();

void transmisieRD160();



#endif /* BMS_COSA_H_ */
