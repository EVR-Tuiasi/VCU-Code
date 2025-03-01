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


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


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
void sendvolt(unsigned int volt);
void USBInit(uint8 UartChannel);
void USBSendCellTemperature(uint8 CellIndex, uint16 Value, uint8 Precision);
void USBSendBMSCellVoltage(uint16 CellIndex, uint16 Value, uint8 Precision);
void USBSendBMSCurrent(uint16 Value, uint8 Precision);
void USBSendAcceleratorPedals(uint16 Value1, uint16 Value2, uint8 Precision);
void USBSendBrakePedal(uint16 Value, uint8 Precision);
void USBSendErrors(void);


#ifdef __cplusplus
}
#endif

#endif

/** @} */
