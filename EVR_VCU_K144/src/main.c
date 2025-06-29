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


uint8 buffTrimitere[16] = {0x00, 0x2C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8 buffPrimire[64] = {0};
volatile int delei;
int curent1,curent2;
int i1,i2;
int v1,v2;

struct biemese icBaterie;

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

    BmsInit();
    parametriiADC();


    //USBInit(0);
    //Icu_EnableNotification(0);


    /*
    buffTrimitere[0]=0;
    buffTrimitere[1]=0x2;
    transmisieRD48();     //RDCFGA
    */


    uint8 buffer[7];
    buffer[0]=12;


    volatile int delayul=1000000;
    while (1)
    {
    	delayul=300000;
    		while(delayul--);
    	i1=BmsGetPackCurrent();
    	//i1*=100;

    	////USBSendBMSCurrent(i1,0);


    	delayul=30000;
   	    	while(delayul--);
       	v1=BmsGetPackVoltage();
       	//v1*=10;



    	buffer[1]=v1>>16;
    	buffer[2]=v1>>8;
    	buffer[3]=v1%256;
    	buffer[4]=i1>>16;
    	buffer[5]=i1>>8;
    	buffer[6]=i1%256;
    	Uart_SyncSend(0, buffer, 7, 10000000);
    }
	while(1);


}
// test

#ifdef __cplusplus
}
#endif

/** @} */
