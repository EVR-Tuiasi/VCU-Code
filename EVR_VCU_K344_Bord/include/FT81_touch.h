#ifndef FT81_TOUCH_H
#define FT81_TOUCH_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
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
/*Touch screen engine registers*/
#define REG_TOUCH_TRANSFORM_A	(RAM_REG + 0x150)
#define REG_TOUCH_TRANSFORM_B	(RAM_REG + 0x154)
#define REG_TOUCH_TRANSFORM_C	(RAM_REG + 0x158)
#define REG_TOUCH_TRANSFORM_D	(RAM_REG + 0x15C)
#define REG_TOUCH_TRANSFORM_E	(RAM_REG + 0x160)
#define REG_TOUCH_TRANSFORM_F	(RAM_REG + 0x164)
#define REG_TOUCH_CONFIG	(RAM_REG + 0x168)
/*Resistive touch engine registers*/
#define REG_TOUCH_TAG		(RAM_REG + 0x12C)
#define REG_TOUCH_TAG_XY	(RAM_REG + 0x128)
#define REG_TOUCH_SCREEN_XY	(RAM_REG + 0x124)
#define REG_TOUCH_DIRECT_Z1Z2	(RAM_REG + 0x190)
#define REG_TOUCH_DIRECT_XY	(RAM_REG + 0x18C)
#define REG_TOUCH_RZ		(RAM_REG + 0x120)
#define REG_TOUCH_RAW_XY	(RAM_REG + 0x11C)
#define REG_TOUCH_RZTHRESH	(RAM_REG + 0x118)
#define REG_TOUCH_OVERSAMPLE	(RAM_REG + 0x114)
#define REG_TOUCH_SETTLE		(RAM_REG + 0x110)
#define REG_TOUCH_CHARGE		(RAM_REG + 0x10C)
#define REG_TOUCH_ADC_MODE		(RAM_REG + 0x108)
#define REG_TOUCH_MODE			(RAM_REG + 0x104)
/*Capacitive touch engine registers not needed*/


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif
