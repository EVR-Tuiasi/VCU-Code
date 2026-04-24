#ifndef FT81_MISC_H
#define FT81_MISC_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "stdint.h"
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
/*Memory spaces defines*/
#define FT81_X_SIZE 	800U
#define FT81_Y_SIZE 	480U
#define RAM_G			0x000000
#define RAM_G_END		0x0FFFFF
#define RAM_DL			0x300000
#define RAM_DL_END		0x301FFF
#define RAM_REG			0x302000
#define RAM_REG_END		0x302FFF
#define RAM_CMD			0x308000
#define RAM_CMD_END		0x308FFF
/*Command defines*/
/*Power modes*/
#define ACTIVE 			0x00
#define	STANDBY 		0x41
#define SLEEP 			0x42
#define PWRDOWN 		0x43
#define PD_ROMS 		0x49
/*Clock and Reset*/
#define CLKEXT 			0x44
#define CLKINT 			0x48
#define CLKSEL			0x61
#define RST_PULSE		0x68
/*Configuration*/
#define PINDRIVE 		0x70
#define PIN_PD_STATE	0x71
/*Special Registers*/
#define REG_TRACKER		0x7000
#define REG_TRACKER_1	0x7004
#define REG_TRACKER_2	0x7008
#define REG_TRACKER_3	0x700C
#define REG_TRACKER_4	0x7010
#define REG_MEDIAFIFO_READ	0x7014
#define REG_MEDIAFIFO_WRITE	0x7018
/*Miscellaneous registers*/
#define REG_CPURESET	(RAM_REG + 0x20)
#define REG_PWM_DUTY	(RAM_REG + 0xD4)
#define REG_PWM_HZ		(RAM_REG + 0xD0)
#define REG_INT_MASK	(RAM_REG + 0xB0)
#define REG_INT_EN		(RAM_REG + 0xAC)
#define REG_INT_FLAGS	(RAM_REG + 0xA8)
#define REG_GPIO_DIR	(RAM_REG + 0x90)
#define REG_GPIO		(RAM_REG + 0x94)
#define REG_GPIOX_DIR	(RAM_REG + 0x98)
#define REG_GPIOX		(RAM_REG + 0x9C)
#define REG_FREQUENCY	(RAM_REG + 0x0C)
#define REG_CLOCK		(RAM_REG + 0x08)
#define REG_FRAMES		(RAM_REG + 0x04)
#define REG_ID			(RAM_REG + 0x00)
#define REG_TRIM		0x10256C
#define REG_SPI_WIDTH	0x180
/*Coprocessor engine registers*/
#define REG_CMD_DL		(RAM_REG + 0x100)
#define REG_CMD_WRITE	(RAM_REG + 0xFC)
#define REG_CMD_READ	(RAM_REG + 0xF8)
#define REG_CMDB_SPACE	(RAM_REG + 0x574)
#define REG_CMDB_WRITE	(RAM_REG + 0x578)

/*REG_DLSWAP definitions*/
#define DLSWAP_FRAME 	0x02
#define DLSWAP_LINE 	0x01

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
void wr8(uint32_t address, uint8_t value);
void wr16(uint32_t address, uint16_t value);
void wr32(uint32_t address, uint32_t value);
uint8_t rd8(uint32_t address);
uint16_t rd16(uint32_t address);
uint32_t rd32(uint32_t address);
void host_command(uint8_t command, uint8_t parameter);

#ifdef __cplusplus
}
#endif

#endif
