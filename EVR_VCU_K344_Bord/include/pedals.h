#ifndef PEDALS_H
#define PEDALS_H

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
#include"Adc.h"

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
#define BRAKE_START_LIMIT 7993U
#define BRAKE_START_VALID 8489U
#define BRAKE_END_VALID 8737U
#define BRAKE_END_LIMIT 9234U

#define ACCEL_1_START_LIMIT 11567U
#define ACCEL_1_START_VALID 12064U
#define ACCEL_1_END_VALID 13603U
#define ACCEL_1_END_LIMIT 14099U

#define ACCEL_2_START_LIMIT 6503U
#define ACCEL_2_START_VALID 7000U
#define ACCEL_2_END_VALID 8092U
#define ACCEL_2_END_LIMIT 8588U

#define BRAKE_DEBOUNCE_BUFFER_SIZE 30U
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct {
	Adc_GroupType AccelerationAdcGroup1, AccelerationAdcGroup2, BrakeAdcGroup;
	Adc_ValueGroupType AccelerationValue1, AccelerationValue2, BrakeValue;
}Pedals;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void PedalsInit(void);
uint16 PedalsGetAccelerationPercent(void);
uint16 PedalsGetAccelerationPercentSensor1(void);
uint16 PedalsGetAccelerationPercentSensor2(void);
uint16 PedalsGetBrakePercent(void);
void PedalsTest(void);

#ifdef __cplusplus
}
#endif

#endif
