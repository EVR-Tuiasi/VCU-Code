#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spi.h"
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
//Battery indicator graphical properties
#define BATTERY_HEIGHT 300
#define BATTERY_WIDTH 200
#define BATTERY_THICKNESS 10
#define BATTERY_X 0
#define BATTERY_Y 150
#define BATTERY_TEXT_X 10
#define BATTERY_TEXT_Y 100
#define BATTERY_FONT_SIZE 31
//Speedometer graphical properties
#define SPEEDOMETER_RADIUS 180
#define SPEEDOMETER_INNER_RADIUS 120
#define SPEEDOMETER_X 50
#define SPEEDOMETER_Y 100
#define SPEEDOMETER_THICKNESS 5
#define SPEEDOMETER_LOWER_THICKNESS 60
#define SPEEDOMETER_FONT_SIZE 31
#define SPEEDOMETER_FONT_SCALE 2
#define SPEEDOMETER_NUM_REFERENCES 10
#define SPEEDOMETER_MAX_VALUE 140
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct{
	Spi_ChannelType Spi_Channel;
}FT81;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
void DisplayInit(void);
void DisplayTest(void);
void VladTest(void);
void SoundTest(void);
void DashboardTest(void);
void DashboardUpdate(uint32 speed, uint32 power, uint32 battery_voltage, uint32 battery_percent, uint32 battery_temp, uint32 inverter_temp);

#ifdef __cplusplus
}
#endif

#endif
