/*
*   (c) Copyright 2020 NXP
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be used strictly
*   in accordance with the applicable license terms.  By expressly accepting
*   such terms or by downloading, installing, activating and/or otherwise using
*   the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms.  If you do not agree to
*   be bound by the applicable license terms, then you may not retain,
*   install, activate or otherwise use the software.
*
*   This file contains sample code only. It is not part of the production code deliverables.
*/

#ifndef UART_DATASEND_H
#define UART_DATASEND_H

#ifdef __cplusplus
extern "C"{
#endif



/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Mcu.h"

#define SEVEN_SEG_NO_RESPONSE 0
#define SEVEN_SEG_NUMBER_TOO_LARGE 1
#define ACCELERATOR_PEDALS_DIFFERENT_OUTPUT 0
#define TEMPERATURE_TOO_HIGH 0
#define BMS_NO_RESPONSE 0
#define BMS_LOW_VOLTAGE 1
#define BMS_HIGH_CONSUMPTION 2
#define PROCESSOR_RESET 0

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

typedef enum{
	BUS_IDLE,
	BUS_BUSY
}BUS_STATE;

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define ERROR 9
//#define MODULE_START 10
#define CELL_TEMP 10
#define CELL_VOLTAGE 11
#define BMS_VOLTAGE 12
#define BMS_CURRENT 13
#define ACCELERATOR_PEDALS 14
#define BRAKE_PEDAL 15
#define SEVEN_SEGMENT 16
#define PROCESSOR 17
#define TIME 18
//#define MODULE_END 19

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


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


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void USBInit(uint8 UartChannel);
void USBSendCellTemperature(uint16 CellIndex, sint32 Value);
void USBSendCellVoltage(uint16 CellIndex, sint32 Value);
void USBSendBMSCurrent(sint32 Value);
void USBSendBMSVoltage(sint32 Value);
void USBSendAcceleratorPedals(uint16 Value1, uint16 Value2);
void USBSendBrakePedal(uint16 Value);
void USBSendErrors(void);
uint8 CRC_calculate(uint8 length);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
