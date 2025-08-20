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
#define BRAKE_START_LIMIT 3319U
#define BRAKE_START_VALID 3646U
#define BRAKE_END_VALID 4577U
#define BRAKE_END_LIMIT 5504U

#define ACCEL_1_START_LIMIT 2342U
#define ACCEL_1_START_VALID 2670U
#define ACCEL_1_END_VALID 4267U
#define ACCEL_1_END_LIMIT 5595U

#define ACCEL_2_START_LIMIT 3417U
#define ACCEL_2_START_VALID 3744U
#define ACCEL_2_END_VALID 5677U
#define ACCEL_2_END_LIMIT 7005U

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
