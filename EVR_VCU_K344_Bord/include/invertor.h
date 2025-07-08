#ifndef INVERTOR_H
#define INVERTOR_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include"Mcu.h"
#include"Can_43_FLEXCAN.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define CURRENT_DEBOUNCE_BUFFER_SIZE 3U
#define VOLTAGE_DEBOUNCE_BUFFER_SIZE 3U

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct{
	uint16 rpm;
	uint16 current;
	uint16 voltage;
	uint8 throttle;
	uint8 controllerTemperature;
	uint8 motorTemperature;
}InverterData;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void CanIf_PrimitMesaj1(PduIdType RxPduId, const PduInfoType * PduInfoPtr);
void CanIf_PrimitMesaj2(PduIdType RxPduId, const PduInfoType * PduInfoPtr);
void InverterInit(void);
uint16 InverterGetRpm(uint8 InverterIndex);
uint16 InverterGetCurrent(uint8 InverterIndex);
uint16 InverterGetVoltage(uint8 InverterIndex);
uint8 InverterGetPedalPercent(uint8 InverterIndex);
uint8 InverterGetControllerTemperature(uint8 InverterIndex);
uint8 InverterGetMotorTemperature(uint8 InverterIndex);
uint8 InverterGetThrottle(uint8 InverterIndex);

#ifdef __cplusplus
}
#endif

#endif
