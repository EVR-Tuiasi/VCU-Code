#ifdef __cplusplus
extern "C" {
#endif


/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "pedal.h"
#include "Adc.h"

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
Pedals Pedalsinstance={ //initializari
		0,1,2,  //channeluri
		0,0,0,  //valori
		10   //eroare
};

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
//initializarea canalelor si valorilor
void PedalsInit(void)
{
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel1, &Pedalsinstance.AccelerationValue1);
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel2, &Pedalsinstance.AccelerationValue2);
	Adc_SetupResultBuffer(Pedalsinstance.BrakeAdcChannel, &Pedalsinstance.BrakeValue);
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel1, &Pedalsinstance.MaxError);
	Adc_SetupResultBuffer(Pedalsinstance.AccelerationAdcChannel2, &Pedalsinstance.MaxError);

}

//functie globala pentru preluarea si stocarea datelor de la pedala de acceleratie. canal ADC + valoarea in volti
uint8 PedalsGetAcceleration(void)
{
	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcChannel1);

	while(1){
	volatile	Adc_StatusType StatusAdc = Adc_GetGroupStatus(Pedalsinstance.AccelerationAdcChannel1);

		if (StatusAdc== ADC_STREAM_COMPLETED)
		{ break; }

	}

	Adc_StartGroupConversion(Pedalsinstance.AccelerationAdcChannel2);

	while(1){
		volatile	Adc_StatusType StatusAdc = Adc_GetGroupStatus(Pedalsinstance.AccelerationAdcChannel2);

			if (StatusAdc== ADC_STREAM_COMPLETED)
			{ break; }
	}


 volatile  uint16 valoare1 = Pedalsinstance.AccelerationValue1; //iau valoarea de la pedala in volt si transform in uint16
 volatile  uint16 valoare2 = Pedalsinstance.AccelerationValue2;

 if (valoare1 < 27 && valoare1 > 20)
 { valoare1 =27;} // fortez ca val min uint16 sa fie 27 pt val1   si are val max 65

 if (valoare1 > 65 && valoare1 < 70)// fortez ca val min uint16 sa fie 27 pt val1   si are val max 65
   { valoare1 =65;}

 /*
 if (valoare1 < 20)   //failed LOW <0.5V
    //to do apelare
 if (valoare1 > 70)   //failed HIGH >4.5V
    // to do apelare
*/

 if (valoare2 < 55 && valoare2 > 30)  // fortez ca val min uint16 sa fie 55 pt val2
  { valoare2 =55;}

 if (valoare2 > 100 && valoare2 > 100)
  { valoare2  =100;  }       // NU E O VALOARE CORECTA E DOAR DE TEST


 /*
 if (valoare2 < 30) //failed LOW <0.5V
	 // to do apelare
 {;}
 if (valoare2 > 90) //failed HIGH >4.5V
	 // to do apelare
 { ; }
 */
   uint8 procente1 =( (uint16) valoare1 * (uint16)100) / (uint16) 65;
   uint8 procente2 =( (uint16) valoare2 * (uint16)100) / (uint16) 100;

   uint8 medie = (procente1 + procente2)/2;

	return medie;


}

//To detect hard braking, a brake system pressure sensor must be used. The threshold must be
//chosen such that there are no locked wheels and the brake pressure is <=  30bar
 // LA FRANA PANA LA 90% E FRANA LIBERA DUPA 90% E NEAPARAT SA INTRE
uint8 PedalsGetBrake(void)
{ Adc_StartGroupConversion(Pedalsinstance.BrakeAdcChannel);
while(1){
volatile	Adc_StatusType StatusAdc = Adc_GetGroupStatus(Pedalsinstance.BrakeAdcChannel);
	if (StatusAdc== ADC_STREAM_COMPLETED)
	{ break; }

   }
Pedalsinstance.BrakeValue = (Pedalsinstance.BrakeValue * 100) / 255;
return  Pedalsinstance.BrakeValue;
}
/*
void PedalsTest(void){
    // COMPAR CU EROAREA. DACA E MAI MARE DECAT EROARE RETURNEZ 0
    // DACA MAI MIC DECAT EROARE RETURNEZ VALOAREA
  uint8 calcul = Pedalsinstance.AccelerationValue1 - Pedalsinstance.AccelerationValue2;
    if (calcul < 0) {
       calcul=calcul*(1);
    }
  if( (calcul/Pedalsinstance.AccelerationValue2)*100 > Pedalsinstance.MaxError)
   return 0;
  //apelez o functie numita implauzibilitate care opreste motoarele dupa 100ms de eroare constanta >10%
  else
	  return medie;
}
*/




#ifdef __cplusplus
}
#endif

/** @} */
