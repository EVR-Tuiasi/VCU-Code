#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "CDD_I2c.h"
#include "Dio.h"
#include "Icu.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "Spi.h"
#include "bms.h"
#include "uart_datasend.h"
#include "uart_error_handling.h"
#include "CDD_Uart.h"
#include "7-segment-display.h"
#include "bms_cosa.h"


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

uint8 DigitNumar1[2] = {0x01, 0x0f};
uint8 DigitNumar2[2] = {0x02, 0x0f};
uint8 DigitNumar3[2] = {0x03, 0x0f};
uint8 DigitNumar4[2] = {0x04, 0x0f};// numarul care va fi afisat pe digit
uint8 test_data[2] = {0x0f, 1}; // comanda test optic

I2c_RequestType test = {0, false, false, false, false, 2, I2C_SEND_DATA, test_data};
I2c_RequestType numarpedigit4 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar4};
I2c_RequestType numarpedigit3 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar3};
I2c_RequestType numarpedigit2 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar2};
I2c_RequestType numarpedigit1 = {0, false, false, false, false, 2, I2C_SEND_DATA, DigitNumar1};

volatile uint8 ok = 0;


uint8 buffTrimitere[64] = {0x00, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8 buffPrimire[64] = {0};
volatile int delei;
int curent1,curent2;
volatile int i1,i2;
volatile int v1,v2;
volatile int32_t value24;
volatile int tensiuneMILIvolti1,tensiuneMILIvolti2, tensiuneMILIvolti3;
uint16 dpec;

struct biemese icBaterie;

int numarulDeDispozitive = NUMARUL_DE_MONITOARE + NUMARUL_DE_SUNTURI;


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void IntrerupereBTN(void){
	ok = 1;
	Dio_WriteChannel(96, 1);
	Dio_WriteChannel(111, 1);

}

void I2c_Callback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(96, 0);
	Dio_WriteChannel(111, 1);
	(void) Event;
	(void) Channel;
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
	Dio_WriteChannel(96, 1);
	ok = 1;
	(void) Event;
	(void) Channel;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


int main(void)
{

    /* Initialize the Mcu driver */
#if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
#elif (MCU_PRECOMPILE_SUPPORT == STD_OFF)
    Mcu_Init(&Mcu_Config_VS_0);
#endif /* (MCU_PRECOMPILE_SUPPORT == STD_ON) */

    /* Initialize the clock tree and apply PLL as system clock */
    Mcu_InitClock(McuClockSettingConfig_0);
#if (MCU_NO_PLL == STD_OFF)
    while ( MCU_PLL_LOCKED != Mcu_GetPllStatus() )
    {
        /* Busy wait until the System PLL is locked */
    }

    Mcu_DistributePllClock();
#endif
    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize all pins using the Port driver */
    Port_Init(NULL_PTR);
    Platform_Init(NULL_PTR);
    Uart_Init(NULL_PTR);
    Spi_Init(NULL_PTR);

    //alt branch

    SRST();

    RDSID();
    flushTX();

    RDCFGA();
    flushTX();

    parametriiADC();
    flushTX();

    RDCFGA();
    flushTX();

    USBInit(0);


    populeazaCMD(0x03, 0xE0);
    transmisieCMD(); //ADCV
/*
    buffTrimitere[0]=0x04;
    buffTrimitere[1]=0x30;
    populeazaCMD(0x04, 0x30);
    transmisieCMD(); //ADCV*/

    uint8 buffer[10];

    uint8 pachete[6]={0x44, 0x46, 0x48, 0x4A};


    volatile int delayul=1000000;
    while (1) {
        for (int i = 0; i <= 3; i++) {
            buffer[0] = 13 + i;
            delayul = 5000;
            while (delayul--) {
                // wait
            }

            buffTrimitere[0] = 0x03;
            buffTrimitere[1] = 0xE0;
            transmisieCMD(); // ADCV

            buffTrimitere[0] = 0;
            buffTrimitere[1] = pachete[i]; // 0x0C
            transmisieCMD();

            tensiuneMILIvolti1 = 15 * (buffPrimire[5] * 256 + buffPrimire[4]) + 150000;
            tensiuneMILIvolti2 = 15 * (buffPrimire[7] * 256 + buffPrimire[6]) + 150000;
            tensiuneMILIvolti3 = 15 * (buffPrimire[9] * 256 + buffPrimire[8]) + 150000;

            if (i == 0) {
                i1 = (buffPrimire[17] << 16) + (buffPrimire[16] << 8) + (buffPrimire[15]);
            } else if (i == 1) {
                value24 = (buffPrimire[14] << 16) | (buffPrimire[13] << 8) | buffPrimire[12];
                if (value24 & 0x800000) {
                    value24 |= 0xFF000000;  // Set upper 8 bits to 1
                } else {
                    value24 &= 0x00FFFFFF;  // Clear upper 8 bits
                }
                v1 = value24;
            }

            buffer[1] = tensiuneMILIvolti1 >> 16;
            buffer[2] = tensiuneMILIvolti1 >> 8;
            buffer[3] = tensiuneMILIvolti1 % 256;

            buffer[4] = tensiuneMILIvolti2 >> 16;
            buffer[5] = tensiuneMILIvolti2 >> 8;
            buffer[6] = tensiuneMILIvolti2 % 256;

            buffer[7] = tensiuneMILIvolti3 >> 16;
            buffer[8] = tensiuneMILIvolti3 >> 8;
            buffer[9] = tensiuneMILIvolti3 % 256;

            //Uart_SyncSend(0, buffer, 10, 10000000);

            if (i==0) {
                i1 *= 5;
                buffer[0] = 13;
                buffer[1] = (i1 >> 24) % 256;
                buffer[2] = (i1 >> 16) % 256;
                buffer[3] = (i1 >> 8)  % 256;
                buffer[4] = i1 % 256;
                buffer[5] = 0x8D;
                Uart_SyncSend(0, buffer, 6, 10000000);
            }
            else if(i==1)
            {
                v1 *= 1;
                buffer[0] = 12;
                buffer[1] = (v1 >> 24) % 256;
                buffer[2] = (v1 >> 16) % 256;
                buffer[3] = (v1 >> 8)  % 256;
                buffer[4] = v1 % 256;
                buffer[5] = 0x8D;
                Uart_SyncSend(0, buffer, 6, 10000000);
            }

            for (int j = 0; j < 3; j++) {
                switch (j)
                {
                    case 0: v1 = tensiuneMILIvolti1; break;
                    case 1: v1 = tensiuneMILIvolti2; break;
                    case 2: v1 = tensiuneMILIvolti3; break;
                    default: break;
                }

                buffer[0] = 11;
                buffer[1] = 0;
                buffer[2] = 3 * i + j;
                buffer[3] = (v1 >> 24) % 256;
                buffer[4] = (v1 >> 16) % 256;
                buffer[5] = (v1 >> 8)  % 256;
                buffer[6] = v1 % 256;
                buffer[7] = 0x8D;

                Uart_SyncSend(0, buffer, 8, 10000000);
            }
        }

    }

}
// test

#ifdef __cplusplus
}
#endif

/** @} */
