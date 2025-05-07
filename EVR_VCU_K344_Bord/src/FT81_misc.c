
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
#include "FT81_misc.h"
#include "Spi.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
#define FT81_SPI_CHANNEL 1

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
void wr8(uint32 address, uint8 value){
	uint8 bufferTrimitere[4] = {0x80 | (uint8)((address >> 16) & 0x3F), (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), (uint8)value};
	uint8 bufferPrimire[4] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 4U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
}

void wr16(uint32 address, uint16 value){//TODO verifica daca pun datele invers sau nu
	uint8 bufferTrimitere[5] = {0x80 | (uint8)((address >> 16) & 0x3F), (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), (uint8)value, (uint8)(value >> 8)};
	uint8 bufferPrimire[5] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 5U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
}

void wr32(uint32 address, uint32 value){
	uint8 bufferTrimitere[7] = {0x80 | (uint8)((address >> 16) & 0x3F), (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), (uint8)value, (uint8)(value >> 8), (uint8)(value >> 16), (uint8)(value >> 24)};
	uint8 bufferPrimire[7] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 7U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
}

uint8 rd8(uint32 address){
	uint8 bufferTrimitere[5] = {0x00 | (uint8)((address >> 16) & 0x3F), (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), 0, 0};
	uint8 bufferPrimire[5] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 5U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
    return bufferPrimire[4];
}

uint16 rd16(uint32 address){
	uint8 bufferTrimitere[6] = {0x00 | (uint8)((address >> 16) & 0x3F), (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), 0, 0, 0};
	uint8 bufferPrimire[6] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 6U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
    return (uint16)bufferPrimire[4] + ((uint16)bufferPrimire[5] << 8);
}

uint32 rd32(uint32 address){//TODO toate returnurile trebuie facute pe dos
	uint8 bufferTrimitere[8] = {(uint8)((address >> 16) & 0x3F), 0x00 | (uint8)((address >> 8) & 0xFF), (uint8)(address & 0xFF), 0, 0, 0, 0, 0};
	uint8 bufferPrimire[8] = {0};
    Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 8U);
    Spi_SyncTransmit(FT81_SPI_CHANNEL);
    return (uint32)bufferPrimire[4] + ((uint32)bufferPrimire[5] << 8) + ((uint32)bufferPrimire[6] << 16) + ((uint32)bufferPrimire[7] << 24);
}

void host_command(uint8 command, uint8 parameter){
	uint8 bufferTrimitere[3] = {0, 0, 0};
	if(command != ACTIVE){
		bufferTrimitere[0] = 0x40 | (command & 0x3F);
		bufferTrimitere[1] = parameter;
		bufferTrimitere[2] = 0;
	}
	uint8 bufferPrimire[3] = {0};
	Spi_SetupEB(FT81_SPI_CHANNEL, bufferTrimitere, bufferPrimire, 3U);
	Spi_SyncTransmit(FT81_SPI_CHANNEL);
}


#ifdef __cplusplus
}
#endif
/** @} */
