
#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include"Mcu.h"
#include"Can_43_FLEXCAN.h"
#include"invertor.h"
#include"Dio.h"

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
InverterData InverterInstance[2];

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
void CanIf_PrimitMesaj1(PduIdType RxPduId, const PduInfoType * PduInfoPtr)
{
	uint16 rpm = 0, current = 0, voltage = 0;
    (void)RxPduId;
    if(PduInfoPtr->SduLength == 8){
    	rpm = ((uint16)PduInfoPtr->SduDataPtr[0]) + (((uint16)PduInfoPtr->SduDataPtr[1])<<8U);
    	current = ((uint16)PduInfoPtr->SduDataPtr[2]) + (((uint16)PduInfoPtr->SduDataPtr[3])<<8U);
    	voltage = ((uint16)PduInfoPtr->SduDataPtr[4]) + (((uint16)PduInfoPtr->SduDataPtr[5])<<8U);
        if(rpm <= 6000U){
        	InverterInstance[0].rpm = rpm;
        }
        if(current <= 4000U){
        	InverterInstance[0].current = current;
        }
        if(voltage <= 1800){
        	InverterInstance[0].voltage = voltage;
        }
    }
}

void CanIf_PrimitMesaj2(PduIdType RxPduId, const PduInfoType * PduInfoPtr)
{
    (void)RxPduId;
	uint8 throttle = 0, controllerTemp = 0, motorTemp = 0;
    if(PduInfoPtr->SduLength == 8){
        throttle = PduInfoPtr->SduDataPtr[0];
        controllerTemp = PduInfoPtr->SduDataPtr[1];
        motorTemp = PduInfoPtr->SduDataPtr[2];
        //these do not need tests, whole uint8 range of values is valid
        InverterInstance[0].throttle = throttle;
        InverterInstance[0].controllerTemperature = controllerTemp;
        InverterInstance[0].motorTemperature = motorTemp;
    }

}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void InverterInit(void){
	Dio_WriteChannel(85, STD_HIGH);
	volatile int i = 1000000;
	while(i--);
	Dio_WriteChannel(88, STD_HIGH);
}

uint16 InverterGetRpm(uint8 InverterIndex){
	if(InverterIndex < 2U){
		if(InverterInstance[InverterIndex].rpm > 6000U){
			return 6000U;
		}
		else{
			return InverterInstance[InverterIndex].rpm;
		}
	}
	else{
		return 0;
	}
}

uint16 InverterGetCurrent(uint8 InverterIndex){
	if(InverterIndex < 2U){
		if(InverterInstance[InverterIndex].current > 4000U){
			return 4000U;
		}
		else{
			return InverterInstance[InverterIndex].current;
		}
	}
	else{
		return 0;
	}
}

uint16 InverterGetVoltage(uint8 InverterIndex){
	if(InverterIndex < 2U){
		if(InverterInstance[InverterIndex].voltage > 1800U){
			return 1800U;
		}
		else{
			return InverterInstance[InverterIndex].voltage;
		}
	}
	else{
		return 0;
	}
}

uint8 InverterGetPedalPercent(uint8 InverterIndex){
	if(InverterIndex < 2U){
		return ((uint16)InverterInstance[InverterIndex].throttle) * 25U / 64U;
	}
	else{
		return 0;
	}
}

uint8 InverterGetControllerTemperature(uint8 InverterIndex){
	if(InverterIndex < 2U){
		if(InverterInstance[InverterIndex].controllerTemperature <= 40U){
			return 0;
		}
		else{
			return InverterInstance[InverterIndex].controllerTemperature - 40U;
		}
	}
	else{
		return 0;
	}
}

uint8 InverterGetMotorTemperature(uint8 InverterIndex){
	if(InverterIndex < 2U){
		if(InverterInstance[InverterIndex].motorTemperature <= 30U){
			return 0;
		}
		else{
			return InverterInstance[InverterIndex].motorTemperature - 30U;
		}
	}
	else{
		return 0;
	}
}
uint8 InverterGetThrottle(uint8 InverterIndex){
	if(InverterIndex < 2U){
		return ((uint16)InverterInstance[InverterIndex].throttle) * 25U / 64U;
	}
	else{
		return 0;
	}
}

#ifdef __cplusplus
}
#endif
/** @} */
