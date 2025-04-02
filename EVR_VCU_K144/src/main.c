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
uint8 buffPrimire[32] = {0};
volatile int delei;
int curent1,curent2;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

void IntrerupereBTN(void){
	ok = 1;
	Dio_WriteChannel(96, 1);
	Dio_WriteChannel(111, 1);

}

void I2c_Callback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(96, 0);
	Dio_WriteChannel(111, 1);
}

void I2c_ErrorCallback(uint8 Event, uint8 Channel){
	Dio_WriteChannel(111, 0);
	Dio_WriteChannel(96, 1);
	ok = 1;
}

void transmisie()
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 24U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}



void transmisieCMD()
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 4U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}

void transmisieRD48()
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 12U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}

void transmisieWR48()
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	    	uint16 dpec = pec10_calc(false,6U, buffTrimitere+4);
	    	buffTrimitere[10] = dpec >> 8;
	    	buffTrimitere[11] = dpec % 256;

	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 12U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}

void transmisieRD160()
{
	#if 1
	    	Port_SetPinMode(9, PORT_MUX_AS_GPIO);
	    	Dio_WriteChannel(37, 0);
	    	delei = 30;
	    	while(delei){
	    		delei--;
	    	}
	    	Dio_WriteChannel(37, 1);
	    	Port_ResetPinMode(9);
	    	delei = 3000;
	    	while(delei){
	    		delei--;
	    	}
	#endif
	    	//comanda fara pec
	        //TODO GRIJA MARE LA LSB SI MSB, acum se trimit pe dos
	        //comanda cu pec


	    	uint16 pec = Pec15_Calc(2U, buffTrimitere);
	    	buffTrimitere[2] = pec >> 8;
	    	buffTrimitere[3] = pec % 256;
	        Spi_SetupEB(0u, buffTrimitere, buffPrimire, 32U);
	#if 0
	    	Dio_WriteChannel(37, 0);
	#endif

	        Spi_SyncTransmit(0);
	#if 0
	    	Dio_WriteChannel(37, 1);
	#endif
	    	delei = 300000;
	    	while(delei){
	    		delei--;
	    	}

	    	//////
}


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
    //Uart_Init(NULL_PTR);
    //I2c_Init(NULL_PTR);
    //Icu_Init(NULL_PTR);
    Spi_Init(NULL_PTR);
    //Icu_EnableNotification(0);
    //002C RDSID
    //

    /*
    buffTrimitere[0]=0;
    buffTrimitere[1]=0x2C;
    transmisie(); //read RDSID

	buffTrimitere[0]=7;
	buffTrimitere[1]=0x22;
	transmisie(); //read COMM reg

	buffTrimitere[0]=0;
	buffTrimitere[1]=0x2C;
	transmisie(); //read SSID

	buffTrimitere[0]=0;
	buffTrimitere[1]=0x10;
	transmisie(); //read all

	buffTrimitere[0]=0;
    buffTrimitere[1]=0x51;
    transmisieRD160();     //RDALLX

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x35;
    transmisieRD160();     //RDVALL

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x11;
    transmisieRD160();     //RDALLR

    buffTrimitere[0]=0x08;
    buffTrimitere[1]=0x60;
    transmisieCMD(); //ADV

    buffTrimitere[0]=0x05;
    buffTrimitere[1]=0x30;
    transmisieCMD(); //ADX

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x0A;
    transmisieRD48();     //RDV1A

	*/

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x27;
    transmisieCMD(); //SRST

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x2;
    transmisieRD48();     //RDCFGA

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x1;

    buffTrimitere[4]=0;
    buffTrimitere[5]=0;
    buffTrimitere[6]=0;
    buffTrimitere[7]=0x5F;
    buffTrimitere[8]=0;
    buffTrimitere[9]=0x10;
    transmisieWR48(); //WRCFGA

    buffTrimitere[0]=0;
    buffTrimitere[1]=0x2;
    transmisieRD48();     //RDCFGA



    buffTrimitere[0]=0x02;
    buffTrimitere[1]=0xE0;
    transmisieCMD(); //ADI1







    buffTrimitere[0]=0;
    buffTrimitere[1]=0x0C;
    transmisieRD160();     //RDALLI
    curent1=buffPrimire[6]<<16+buffPrimire[5]<<8+buffPrimire[4];


    buffTrimitere[0]=0;
    buffTrimitere[1]=0x4C;
    transmisieRD160();     //RDALLA
    curent2=buffPrimire[6]<<16+buffPrimire[5]<<8+buffPrimire[4];



	while(1);


#if 0
    USBInit(0);
    //SevenSegmentInit();
    //SevSegGrTest(0);
    ErrorsSet(BMS_VOLTAGE, BMS_NO_RESPONSE);
    ErrorsSet(SEVEN_SEGMENT, SEVEN_SEG_NO_RESPONSE);
    ErrorsSet(SEVEN_SEGMENT, SEVEN_SEG_NUMBER_TOO_LARGE);
    ErrorsSet(BRAKE_PEDAL, ACCELERATOR_PEDALS_DIFFERENT_OUTPUT);
    ErrorsSet(BMS_CURRENT, BMS_NO_RESPONSE);

    while(1){
    	volatile int i = 100000;
    	while(i)
    		i--;
    	USBSendErrors();
    }
#endif

}
// test

#ifdef __cplusplus
}
#endif

/** @} */
