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
#include "stdint.h"
#include "stdbool.h"
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

//NEW VALUES

//Display Witnesses
#define INVERTER_ERR								1U
#define BATTERY_ERR									2U
#define ACCEL_ERR									3U
#define BRAKE_ERR									4U
#define ALL_ERRS									5U

//Critical Warnings
#define WARNING_CELL_X								450U
#define WARNING_CELL_Y								86U
#define WARNING_CELL_WIDTH							270U
#define WARNING_CELL_HEIGHT							104U
#define WARNING_CELL_DIAGONAL_WIDTH					313U
#define WARNING_CELL_DIAGONAL_HEIGHT				135U

#define WARNING_CELL_TEMP_X							480U
#define WARNING_CELL_TEMP_Y							195U
#define WARNING_CELL_TEMP_WIDTH						220U
#define WARNING_CELL_TEMP_HEIGHT					95U
#define WARNING_CELL_TEMP_DIAGONAL_WIDTH			283U
#define WARNING_CELL_TEMP_DIAGONAL_HEIGHT			140U

#define WARNING_CURRENT_X							485U
#define WARNING_CURRENT_Y							295U
#define WARNING_CURRENT_HEIGHT						105U
#define WARNING_CURRENT_WIDTH						228U
#define WARNING_CURRENT_Y_OFFSET					41U
#define WARNING_CURRENT_X_OFFSET					50U

//Battery Inner Filling
#define BATTERY_FILL_HEIGHT 						429U
#define BATTERY_FILL_MAX_HEIGHT 					67U

//Battery Outer Border
#define BORDER_THICKNESS							5U
#define BATTERY_BORDER_X							763U
#define BATTERY_BORDER_Y							60U
#define BATTERY_BORDER_WIDTH						37U
#define BATTERY_BORDER_HEIGHT						395U

//Motor Temperature
#define MOTOR_LIMITED_TEMP							60U
#define MOTOR_TEMP_WIDTH							155U
#define MOTOR_TEMP_X								800U
#define MOTOR_TEMP_Y								458U
#define MOTOR_TEMP_DIAG_X							587U
#define MOTOR_TEMP_DIAG_Y							434U
#define MOTOR_TEMP_DIAG_WIDTH						84U

#define INVERTER_TEMP_X								0U
#define INVERTER_TEMP_Y								458U
#define INVERTER_TEMP_WIDTH							150U
#define INVERTER_TEMP_DIAG_X						211U
#define INVERTER_TEMP_DIAG_Y						434U
#define INVERTER_TEMP_DIAG_WIDTH					86U


//Lower Borders
#define INVERTER_HORIZONTAL_BORDER		  			0U
#define INVERTER_HORIZONTAL_BORDER_WIDTH			150U
#define LOWER_HORIZONTAL_BORDER_HEIGHT				455U
#define MIDDLE_HORIZONTAL_BORDER_X					210U
#define MIDDLE_HORIZONTAL_BORDER_Y					430U
#define MIDDLE_HORIZONTAL_BORDER_WIDTH				375U
#define	MOTOR_HORIZONTAL_BORDER						645U
#define	MOTOR_HORIZONTAL_BORDER_WIDTH				155U

#define LOWER_INVERTER_DELIMITER					300U
#define LOWER_MOTOR_DELIMITER						500U

#define MAX_X										800U
#define MAX_Y										480U

//Data Underlines

#define TEMPERATURE_UNDERLINE_X						480U
#define TEMPERATURE_UNDERLINE_Y						290U
#define TEMPERATURE_UNDERLINE_WIDTH					220U
#define TEMPERATURE_UNDERLINE_HEIGHT_OFFSET			337U


#define VOLTAGE_UNDERLINE_X							450U
#define VOLTAGE_UNDERLINE_Y							190U
#define VOLTAGE_UNDERLINE_WIDTH						270U
#define VOLTAGE_UNDERLINE_HEIGHT_OFFSET				227U
#define UNDERLINE_END								764U

//Warning lights Space

#define WARNING_BORDER_X							200U
#define WARNING_BORDER_Y							81U
#define WARNING_BORDER_WIDTH						400U
#define WARNING_BORDER_HEIGHT						31U

//Warning lights boxes

#define TL_INVERTER_CORNER							210U
#define BR_INVERTER_CORNER							299U

#define TL_BATTERY_CORNER							304U
#define BR_BATTERY_CORNER							400U

#define TL_ACCEL_CORNER								405U
#define BR_ACCEL_CORNER								495U

#define TL_BRAKE_CORNER								500U
#define BR_BRAKE_CORNER								590U

#define BOX_HEIGHT									55U
#define BOX_HEIGHT_OFFSET							21U

//Upper Borders

#define UPPER_BORDER_X								100U
#define UPPER_BORDER_Y								45U
#define UPPER_BORDER_WIDTH							700U
#define UPPER_BORDER_OFFSET							50U
#define UPPER_VERTICAL_LEFT_LINE					255U
#define UPPER_VERTICAL_MIDDLE_LINE					400U
#define UPPER_VERTICAL_RIGHT_LINE					540U

//Speedometer

#define SPEEDOMETER_MAX_VALUE						150U
#define SPEEDOMETER_MIN_VALUE				 	 	0U

#define LOWER_BARS_HEIGHT							437U
#define LOWER_BARS_ENDS								480U

#define EDGE_STRIP_ENDS								480U

//Speedometer's Cursor

#define INNER_RADIUS								130U
#define OUTER_RADIUS								195U
#define CENTER_X									220U
#define CENTER_Y									300U
#define PI											3.1415926535
#define MIDDLE_RING									3010U

/* Speedometer Hash Marks */

#define HASH1_X 									48U
#define HASH1_Y 									377U
#define HASH1_END_X 								58U
#define HASH1_END_Y 								372U

#define HASH2_X 									29U
#define HASH2_Y 									305U
#define HASH2_END_X 								42U
#define HASH2_END_Y 								305U

#define HASH3_X 									39U
#define HASH3_Y 									234U
#define HASH3_END_X 								50U
#define HASH3_END_Y 								238U

#define HASH4_X 									71U
#define HASH4_Y 									172U
#define HASH4_END_X 								81U
#define HASH4_END_Y 								179U

#define HASH5_X 									131U
#define HASH5_Y 									121U
#define HASH5_END_X 								138U
#define HASH5_END_Y 								132U

#define HASH6_X 									221U
#define HASH6_Y 									99U
#define HASH6_END_X 								221U
#define HASH6_END_Y 								113U

#define HASH7_X 									310U
#define HASH7_Y 									123U
#define HASH7_END_X 								303U
#define HASH7_END_Y 								133U

#define HASH8_X 									367U
#define HASH8_Y 									171U
#define HASH8_END_X 								359U
#define HASH8_END_Y 								178U

#define HASH9_X 									400U
#define HASH9_Y 									235U
#define HASH9_END_X 								390U
#define HASH9_END_Y 								240U

#define HASH10_X 									412U
#define HASH10_Y 									305U
#define HASH10_END_X 								397U
#define HASH10_END_Y 								305U

#define HASH11_X 									391U
#define HASH11_Y 									378U
#define HASH11_END_X 								381U
#define HASH11_END_Y 								372U

/* ACCEL BAR */

#define ACCEL_BAR_ORIGIN_X							544U
#define ACCEL_BAR_ORIGIN_Y							45U
#define ACCEL_BAR_END_X								750U

#define BRAKE_BAR_ORIGIN_X							253U
#define BRAKE_BAR_ORIGIN_Y							45U


/* Font Size Defines */

#define LARGE_FONT									31U
#define MEDIUM_FONT									30U
#define SMALL_FONT									23U
#define CELSIUS_SYMBOL								27U

/* Word Position Defines */

#define SPEED_HUNDREDS								114U
#define SPEED_TENS									130U
#define SPEED_UNITS									175U
#define SPEED_TEXT_Y								170U
#define SPEED_TEXT_SMALL_Y							185U

#define KM_POSITION_X								157U
#define KM_POSITION_Y								326U

#define BATTERY_HUNDREDS							190U
#define BATTERY_TENS								205U
#define BATTERY_UNITS								221U
#define BATTERY_TEXT_POSITION_Y						24U

#define INVERTER_TEXT_POSITION_X					5U
#define INVERTER_TEXT_POSITION_Y					410U
#define INVERTER_TEMP_POSITION_X					212U
#define INVERTER_TEMP_POSITION_Y					433U

#define WARNING_TEXT_POSITION_Y						55U
#define WARNING_TEXT_POSITION_X						210U

#define PEDALS_TEXT_POSITION_X						265U
#define PEDALS_TEXT_POSITION_Y						1U

#define CELL_VOLTAGE_POSITION_X						460U
#define CELL_VOLTAGE_POSITION_Y						90U

#define CELL_TEMPERATURE_POSITION_X					478U
#define CELL_TEMPERATURE_POSITION_Y					192U

#define TOTAL_CURRENT_POSITION_X					500U
#define TOTAL_CURRENT_POSITION_Y					290U

#define TOTAL_VOLTAGE_POSITION_X					80U
#define TOTAL_VOLTAGE_POSITION_Y					52U

#define MOTOR_TEMP_POSITION_X						10U
#define MOTOR_TEMP_POSITION_Y						433U

#define TIME_TEXT_POSITION_X						327U
#define TIME_TEXT_POSITION_Y						435U

#define MOTOR_TEXT_POSITION_X						150U
#define MOTOR_TEXT_POSITION_Y						410U

//Max values for current, voltage, temperatures

#define TOTAL_CURRENT_MAX_VALUE						300U
#define CELL_VOLTAGE_MAX_VALUE						412U
#define CELL_TEMPERATURE_MAX_VALUE					600U

#define BRAKE_START									56U

/*==================================================================================================
 *                                             ENUMS
==================================================================================================*/

/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct{
	Spi_ChannelType Spi_Channel;
}FT81;

typedef struct{
	bool Inverter;
	bool Battery;
	bool Acceleration;
	bool Brake;
}Witnesses_t;


/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
==================================================================================================*/
void Display_Init(void);
//void ImageTest(void);
void SoundTest(void);
void Display_Test(void);
void Display_Update(uint8_t Acceleration, uint8_t Brake, uint8_t Battery_Percentage, uint16_t Motor_Temperature, uint16_t Inverter_Temperature, uint8_t Speed, uint16_t Cell_Voltage, uint16_t Cell_Temperature, uint16_t Total_Current, uint16_t Total_Voltage, uint8_t Minutes, uint8_t Seconds, uint32_t Miliseconds);
//void trailingArray(void);

#ifdef __cplusplus
}
#endif

#endif
