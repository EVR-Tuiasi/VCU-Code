#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
==================================================================================================*/
#include "display.h"
#include "FT81_misc.h"
#include "FT81_display.h"
#include "FT81_sound.h"
#include "FT81_touch.h"
#include "Dio.h"
#include <stdbool.h>
#include <math.h>

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

Witnesses_t Witnesses = {.Inverter = false,
		.Battery = false,
		.Acceleration = false,
		.Brake = false
};

volatile uint8_t Witness_Delay = 0;
double trail_x[151];
double trail_y[151];



//static volatile uint16_t x_memory_outer[151];
//static volatile uint16_t y_memory_outer[151];
//static volatile uint16_t x_memory_inner[151];
//static volatile uint16_t y_memory_inner[151];

//const uint16_t x_memory_outer[] = {46, 44, 42, 40, 39, 37, 36, 35, 34, 33, 32, 32, 31, 31, 31, 31, 31, 31, 31, 32, 32, 33, 33, 34, 36, 37, 38, 39, 41, 42, 44, 46, 48, 50, 53, 55, 58, 61, 64, 66, 69, 72, 75, 79, 82, 85, 89, 93, 97, 100, 104, 108, 112, 116, 120, 125, 129, 133, 138, 142, 147, 152, 156, 161, 166, 170, 175, 180, 185, 190, 195, 200, 205, 209, 214, 219, 224, 229, 234, 239, 244, 249, 254, 259, 264, 269, 273, 278, 283, 288, 292, 297, 302, 306, 310, 314, 318, 323, 327, 331, 335, 339, 342, 346, 350, 354, 357, 361, 364, 367, 370, 373, 376, 379, 381, 384, 386, 388, 390, 392, 394, 396, 398, 400, 401, 402, 403, 404, 405, 406, 407, 408, 408, 408, 408, 408, 408, 408, 408, 408, 407, 406, 405, 404, 403, 402, 401, 399, 397, 396, 394};
//const uint16_t y_memory_outer[] = {373, 368, 363, 358, 354, 349, 344, 339, 334, 329, 323, 318, 313, 308, 303, 298, 293, 288, 283, 278, 272, 267, 262, 257, 252, 247, 242, 237, 232, 228, 223, 218, 213, 209, 204, 200, 196, 191, 187, 183, 178, 174, 170, 167, 163, 159, 156, 152, 149, 145, 142, 139, 136, 134, 131, 128, 125, 123, 121, 119, 117, 115, 113, 112, 110, 108, 107, 106, 105, 104, 104, 103, 103, 102, 102, 102, 102, 102, 103, 103, 104, 104, 105, 106, 107, 108, 110, 112, 113, 115, 116, 118, 121, 123, 126, 128, 131, 133, 136, 139, 142, 145, 149, 152, 156, 159, 163, 166, 170, 174, 178, 183, 187, 191, 196, 200, 205, 209, 214, 218, 223, 227, 232, 237, 242, 247, 252, 257, 262, 267, 272, 277, 282, 287, 292, 298, 303, 308, 313, 318, 323, 328, 333, 338, 343, 348, 353, 358, 363, 367, 372};
//static uint16_t x_memory_inner[] = {100, 99, 97, 96, 95, 94, 93, 93, 92, 92, 91, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 92, 92, 92, 93, 94, 95, 96, 97, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114, 116, 118, 120, 123, 125, 127, 130, 132, 135, 137, 140, 143, 146, 149, 151, 154, 157, 160, 163, 166, 169, 173, 176, 179, 182, 186, 189, 192, 196, 199, 203, 206, 210, 213, 216, 219, 223, 226, 230, 233, 237, 240, 244, 247, 250, 253, 257, 260, 263, 266, 270, 273, 276, 279, 282, 285, 288, 290, 293, 296, 299, 302, 304, 307, 310, 312, 314, 317, 319, 321, 323, 325, 327, 329, 330, 332, 334, 336, 337, 338, 339, 341, 342, 343, 344, 345, 346, 347, 347, 348, 349, 349, 349, 349, 349, 349, 349, 349, 349, 349, 348, 347, 347, 346, 346, 345, 344, 343, 342, 340, 339};
//static uint16_t y_memory_inner[] = {340, 337, 334, 331, 328, 324, 321, 318, 314, 311, 307, 304, 301, 297, 294, 290, 287, 284, 280, 277, 273, 270, 267, 263, 260, 257, 254, 250, 247, 243, 240, 237, 234, 231, 228, 225, 222, 219, 217, 214, 211, 208, 205, 203, 200, 198, 196, 193, 191, 189, 187, 185, 183, 181, 179, 177, 175, 174, 172, 171, 170, 169, 167, 166, 165, 164, 163, 162, 162, 161, 161, 160, 160, 160, 160, 160, 160, 160, 160, 160, 161, 161, 162, 162, 163, 164, 165, 166, 167, 168, 170, 171, 173, 174, 176, 177, 179, 181, 183, 185, 187, 189, 191, 193, 196, 198, 200, 203, 205, 208, 211, 214, 216, 219, 222, 225, 228, 231, 234, 237, 240, 244, 247, 250, 253, 257, 260, 263, 267, 270, 274, 277, 280, 283, 287, 290, 294, 298, 301, 304, 307, 310, 314, 317, 321, 324, 328, 331, 334, 337, 340};
/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
 *                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
==================================================================================================*/
void Display_Init(void){
	/*Cod nebun display*/
	Dio_WriteChannel(11, 0);
	volatile int delei = 3000000;
	while(delei){
		delei--;
	}
	Dio_WriteChannel(11, 1);
	delei = 3000000;
	while(delei){
		delei--;
	}
	host_command(CLKEXT, 0);//send command to "CLKEXT" to FT81X
	delei = 3000000;
	while(delei){
		delei--;
	}
	host_command(CLKSEL, 0x86);//select the system clock frequency
	delei = 3000000;
	while(delei){
		delei--;
	}
	/*host_command(RST_PULSE, 0);//send host command "RST_PULSE" to reset
	delei = 3000000;
	while(delei){
		delei--;
	}*/
	host_command(ACTIVE, 0);//send host command "ACTIVE" to wake up
	/*delei = 30000000;
	while(delei){
		delei--;
	}*/
	while (0x7C != rd8(REG_ID)); //Wait till clock is on
	while (0x0 != rd8(REG_CPURESET)); //Check if EVE is in working status.
	/* Configure display registers - demonstration for WQVGA resolution, modified for 800x480*/
	wr16(REG_HCYCLE, 928);
	wr16(REG_HOFFSET, 88);
	wr16(REG_HSYNC0, 0);
	wr16(REG_HSYNC1, 48);
	wr16(REG_VCYCLE, 525);
	wr16(REG_VOFFSET, 32);
	wr16(REG_VSYNC0, 0);
	wr16(REG_VSYNC1, 3);
	wr8(REG_SWIZZLE, 0);
	wr8(REG_PCLK_POL, 1);
	wr8(REG_CSPREAD, 1);
	wr16(REG_HSIZE, 800);
	wr16(REG_VSIZE, 480);
	wr8(REG_DITHER, 1);

	/* write first display list */
	wr32(RAM_DL+0,clear_color_rgb(0,0,0));
	wr32(RAM_DL+4,clear(1,1,1));
	wr32(RAM_DL+8,display());
	wr8(REG_DLSWAP,DLSWAP_FRAME);//display list swap
	wr8(REG_GPIO_DIR,0x80);//| rd8(REG_GPIO_DIR));
	wr8(REG_GPIO,0x80);// | rd8(REG_GPIO));//enable display bit
	wr8(REG_PCLK,2);//after this display is visible on the LCD
	trailingArray();
}

void SoundTest(void){
	while(1){
		wr8(REG_VOL_SOUND,0xFF);
		wr32(REG_GPIOX_DIR, 0x00008004);
		volatile uint32_t delei;
		while(1){
			wr32(REG_GPIOX, 0x00008004); // enable amp
			wr16(REG_SOUND, (0x6C<< 8) | 0x41);
			wr8(REG_PLAY, 1);
			delei = 10000000;
			while(delei--);
			wr32(REG_GPIOX, 0x00008000); // disable amp
			wr8(REG_PLAY, 0);
			wr16(REG_SOUND, 0x0);
			wr8(REG_PLAY, 1);
			delei = 10000000;
			while(delei--);
		}
	}
}

void trailingArray(){
	double theta = 0;
	double deg = 0;
	for(int i=0; i<151; i++){
		deg = 203.0 - ((double)(i / 160.0)) * 241.0;
		if(deg < 0){
			deg += 360.0;
		}
		theta = deg * PI / 180.0;
		trail_x[i] = cos(theta);
		trail_y[i] = sin(theta);
	}
}

void Display_Test(){
	static uint8_t Battery_Percentage = 0, Speed = 0, Brake = 0, Acceleration = 0;
	static uint16_t Inverter_Temperature = 0, Motor_Temperature = 0, Cell_Voltage = 0, Cell_Temperature = 0, Total_Current = 0, Total_Voltage = 0;

	static uint32_t milis = 0;
	static uint8_t seconds = 0;
	static uint8_t mins = 0;
	volatile uint32_t delay = 35000;

	milis++;
	while(delay--);
	Display_Update(Acceleration, Brake, Battery_Percentage, Motor_Temperature, Inverter_Temperature, Speed, Cell_Voltage, Cell_Temperature, Total_Current, Total_Voltage, mins, seconds, milis);
	Battery_Percentage++;
	Motor_Temperature++;
	Inverter_Temperature++;
	Cell_Voltage+=2;
	Cell_Temperature+=3;
	Battery_Percentage %= 101;
	Motor_Temperature %= 100;
	Inverter_Temperature %= 100;
	Cell_Voltage %= 999;
	Cell_Temperature %= 999;
	Speed++;
	Speed %= 151;
	Total_Current++;
	Total_Current %= 1000;
	Total_Voltage += 2;
	Total_Voltage %= 999;
	Acceleration++;
	Acceleration %= 110;
	Brake++;
	Brake %= 110;
	Witness_Delay++;
	Witness_Delay %= 251;
	seconds = (milis / 1000U) % 60;
	mins = milis / 60000U;

	if(Witness_Delay < 50U)
	{
		Witnesses.Inverter = true;
		Witnesses.Battery = false;
		Witnesses.Acceleration = false;
		Witnesses.Brake = false;
	}

	else if(Witness_Delay >= 50U && Witness_Delay < 100U)
	{
		Witnesses.Inverter = false;
		Witnesses.Battery = true;
		Witnesses.Acceleration = false;
		Witnesses.Brake = false;
	}

	else if(Witness_Delay >= 100U && Witness_Delay < 150U)
	{
		Witnesses.Inverter = false;
		Witnesses.Battery = false;
		Witnesses.Acceleration = true;
		Witnesses.Brake = false;
	}

	else if(Witness_Delay >= 150U && Witness_Delay < 200U)
	{
		Witnesses.Inverter = false;
		Witnesses.Battery = false;
		Witnesses.Acceleration = false;
		Witnesses.Brake = true;
	}

	else
	{
		Witnesses.Inverter = false;
		Witnesses.Battery = false;
		Witnesses.Acceleration = false;
		Witnesses.Brake = false;
	}


}

void Display_Update(uint32_t Acceleration, uint8_t Brake, uint8_t Battery_Percentage, uint16_t Motor_Temperature, uint16_t Inverter_Temperature, uint8_t Speed, uint16_t Cell_Voltage, uint16_t Cell_Temperature, uint32_t Total_Current, uint32_t Total_Voltage, uint8_t Minutes, uint8_t Seconds, uint32_t Miliseconds){
	uint32_t index = 0;
	uint8_t Red, Green, Blue = 0;
	//uint16_t x_memory_inner[151];
	//uint16_t y_memory_inner[151];

	volatile uint32_t Height_Offset, Width_Offset;

	if(Acceleration > 100U)
	{
		Acceleration = 100U;
	}

	if(Brake > 100U)
	{
		Brake = 100U;
	}

	if(Battery_Percentage > 100U)
	{
		Battery_Percentage = 100U;
	}

	if(Motor_Temperature > 99U)
	{
		Motor_Temperature = 99U;
	}

	if(Inverter_Temperature > 99U)
	{
		Inverter_Temperature = 99U;
	}

	if(Speed > 150U)
	{
		Speed = 150U;
	}

	if(Cell_Voltage > 999U)
	{
		Cell_Voltage = 999U;
	}

	if(Cell_Temperature > 999U)
	{
		Cell_Temperature = 999U;
	}

	if(Total_Current > 999U)
	{
		Total_Current = 999U;
	}

	if(Total_Voltage > 999U)
	{
		Total_Voltage = 999U;
	}

	if(rd8(REG_DLSWAP) == 0){
		wr32(RAM_DL + (index+=4), clear(1, 1, 1));
		wr32(RAM_DL + (index+=4), vertex_format(0));

		/* CRITICAL ERRORS WARNINGS */

		if(Total_Current >= 300U)
		{
			wr32(RAM_DL + (index+=4), color_rgb(255, 0, 0));
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_R));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CURRENT_X, WARNING_CURRENT_Y + WARNING_CURRENT_HEIGHT));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CURRENT_X, WARNING_CURRENT_Y));
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_R));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CURRENT_X + WARNING_CURRENT_WIDTH, WARNING_CURRENT_Y));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CURRENT_X + WARNING_CURRENT_WIDTH + WARNING_CURRENT_X_OFFSET, WARNING_CURRENT_Y + WARNING_CURRENT_Y_OFFSET));
		}

		if(Cell_Temperature >= 600U)
		{
			wr32(RAM_DL + (index+=4), color_rgb(255, 0, 0));
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_R));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_TEMP_X, WARNING_CELL_TEMP_Y));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_TEMP_X, WARNING_CELL_TEMP_Y + WARNING_CELL_TEMP_HEIGHT));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_TEMP_X + WARNING_CELL_TEMP_WIDTH, WARNING_CELL_TEMP_Y + WARNING_CELL_TEMP_HEIGHT));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_TEMP_X + WARNING_CELL_TEMP_DIAGONAL_WIDTH, WARNING_CELL_TEMP_Y + WARNING_CELL_TEMP_DIAGONAL_HEIGHT));
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_R));
			wr32(RAM_DL + (index+=4), vertex2f(724, WARNING_CELL_TEMP_Y - BORDER_THICKNESS));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_TEMP_X + WARNING_CELL_TEMP_DIAGONAL_WIDTH, WARNING_CELL_TEMP_WIDTH));
		}

		if(Cell_Voltage >= 412U)
		{
			wr32(RAM_DL + (index+=4), color_rgb(255, 0, 0));
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_R));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_X, WARNING_CELL_Y));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_X, WARNING_CELL_Y + WARNING_CELL_HEIGHT));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_X + WARNING_CELL_WIDTH, WARNING_CELL_Y + WARNING_CELL_HEIGHT));
			wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_X + WARNING_CELL_DIAGONAL_WIDTH, WARNING_CELL_Y + WARNING_CELL_DIAGONAL_HEIGHT));
		}

		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_CELL_X + 320, WARNING_CELL_Y - 16));
		wr32(RAM_DL + (index+=4), vertex2f(MAX_X, MAX_Y));

		/* END FOR CRITICAL ERRORS */

		wr32(RAM_DL + (index+=4), color_rgb(80, 80, 80)); //start of Upper Indicator Space
		wr32(RAM_DL + (index+=4), begin(RECTS));

		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X, UPPER_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X + UPPER_BORDER_WIDTH, UPPER_BORDER_Y + BORDER_THICKNESS));

		wr32(RAM_DL + (index+=4), line_width(60));
		wr32(RAM_DL + (index+=4), begin(LINES));
		//Diagonal ends
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X, UPPER_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X - UPPER_BORDER_OFFSET, 0));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X + UPPER_BORDER_WIDTH, UPPER_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_BORDER_X + UPPER_BORDER_WIDTH + UPPER_BORDER_OFFSET, 0));

		//Vertical upper indicators
		wr32(RAM_DL + (index+=4), line_width(64));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_MIDDLE_LINE, 0));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_MIDDLE_LINE, UPPER_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_LEFT_LINE, 0));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_LEFT_LINE, UPPER_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_RIGHT_LINE, 0));
		wr32(RAM_DL + (index+=4), vertex2f(UPPER_VERTICAL_RIGHT_LINE, UPPER_BORDER_Y));

		//Speedometer: Design

		/*x_memory_inner[Speed] = CENTER_X + ((uint32_t)(x_memory_outer[Speed] - CENTER_X) * (188/195));
		y_memory_inner[Speed] = CENTER_Y + ((uint32_t)(y_memory_outer[Speed] - CENTER_Y) * (188/195));*/

		//Outer ring
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), stencil_op(INCR, INCR));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), point_size(MIDDLE_RING));
		wr32(RAM_DL + (index+=4), begin(POINTS));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y - 10));
		wr32(RAM_DL + (index+=4), point_size(OUTER_RADIUS*16));
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y - 10));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), point_size(OUTER_RADIUS*16 - 6*16));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y - 10));
		wr32(RAM_DL + (index+=4), stencil_func(GREATER, 2, 255));
		wr32(RAM_DL + (index+=4), color_rgb(247, 198, 0));
		if(Speed < 75U){
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
			wr32(RAM_DL + (index+=4), vertex2f(0, 0));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + (OUTER_RADIUS - 5) * trail_x[Speed], CENTER_Y - (OUTER_RADIUS) * trail_y[Speed] - 2));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + INNER_RADIUS * trail_x[Speed], CENTER_Y - INNER_RADIUS * trail_y[Speed] - 9));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, EDGE_STRIP_ENDS));
		}
		else{
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_A));
			wr32(RAM_DL + (index+=4), vertex2f(EDGE_STRIP_ENDS, 0));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + (OUTER_RADIUS) * trail_x[Speed], CENTER_Y - (OUTER_RADIUS) * trail_y[Speed] - 4));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + INNER_RADIUS * trail_x[Speed], CENTER_Y - INNER_RADIUS * trail_y[Speed] - 8));
			wr32(RAM_DL + (index+=4), vertex2f(0, EDGE_STRIP_ENDS));
			wr32(RAM_DL + (index+=4), begin(RECTS));
			wr32(RAM_DL + (index+=4), vertex2f(80, 320));
			wr32(RAM_DL + (index+=4), vertex2f(120, 380));

		}

		/*ALTERNATE WITHOUT SIN AND COS*/

		/*if(Speed < 75U){
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
			wr32(RAM_DL + (index+=4), vertex2f(0, 0));
			wr32(RAM_DL + (index+=4), vertex2f(x_memory_outer[Speed], y_memory_outer[Speed]));
			wr32(RAM_DL + (index+=4), vertex2f(x_memory_inner[Speed], y_memory_inner[Speed]));
			wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, EDGE_STRIP_ENDS));
		}
		else{
			wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_A));
			wr32(RAM_DL + (index+=4), vertex2f(EDGE_STRIP_ENDS, 0));
			wr32(RAM_DL + (index+=4), vertex2f(x_memory_outer[Speed], y_memory_outer[Speed]));
			wr32(RAM_DL + (index+=4), vertex2f(x_memory_inner[Speed], y_memory_inner[Speed]));
			wr32(RAM_DL + (index+=4), vertex2f(0, EDGE_STRIP_ENDS));
			//wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, EDGE_STRIP_ENDS));
		}*/

		wr32(RAM_DL + (index+=4), clear(0, 1, 0));
		wr32(RAM_DL + (index+=4), restore_context());

		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), line_width(48));
		/*wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
		//wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + INNER_RADIUS * trail_x[0], CENTER_Y - INNER_RADIUS * trail_y[0] - 10));
		//wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + (OUTER_RADIUS - 6) * trail_x[0], CENTER_Y - (OUTER_RADIUS) * trail_y[0] - 3));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, 350));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + INNER_RADIUS * trail_x[Speed], CENTER_Y - INNER_RADIUS * trail_y[Speed] - 10));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + (OUTER_RADIUS - 6) * trail_x[Speed], CENTER_Y - (OUTER_RADIUS) * trail_y[Speed] - 3));*/

		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), begin(LINES));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + INNER_RADIUS * trail_x[Speed], CENTER_Y - INNER_RADIUS * trail_y[Speed] - 10));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X + (OUTER_RADIUS - 6) * trail_x[Speed], CENTER_Y - (OUTER_RADIUS) * trail_y[Speed] - 3));
		/*x_memory_outer[Speed] = CENTER_X + (OUTER_RADIUS - 6) * trail_x[Speed];
		y_memory_outer[Speed] = CENTER_Y - (OUTER_RADIUS) * trail_y[Speed] - 3;
		x_memory_inner[Speed] = CENTER_X + INNER_RADIUS * trail_x[Speed];
		y_memory_inner[Speed] = CENTER_Y - INNER_RADIUS * trail_y[Speed] - 10;*/

		//wr32(RAM_DL + (index+=4), vertex2f(x_memory_outer[Speed], y_memory_outer[Speed]));
		//wr32(RAM_DL + (index+=4), vertex2f(x_memory_inner[Speed], y_memory_inner[Speed]));

		wr32(RAM_DL + (index+=4), restore_context());

		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), point_size(INNER_RADIUS*16));
		wr32(RAM_DL + (index+=4), begin(POINTS));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y - 10));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), point_size(INNER_RADIUS*16 - 4*16));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y - 10));
		wr32(RAM_DL + (index+=4), restore_context());

		//Speedometer delimiter
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
		wr32(RAM_DL + (index+=4), vertex2f(0, CENTER_Y + 90));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X, CENTER_Y + 40));
		wr32(RAM_DL + (index+=4), vertex2f(CENTER_X * 2, CENTER_Y + 90));
		wr32(RAM_DL + (index+=4), restore_context());

		//Inner Text
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), bitmap_handle(31));
		wr32(RAM_DL + (index+=4), bitmap_size(0, 0, 0, 100, 160));
		wr32(RAM_DL + (index+=4), bitmap_transform_a(64));
		wr32(RAM_DL + (index+=4), bitmap_transform_e(64));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		if(Speed >= 100U){
			wr32(RAM_DL + (index+=4), bitmap_transform_a(72));
			wr32(RAM_DL + (index+=4), bitmap_transform_e(72));
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_HUNDREDS, SPEED_TEXT_SMALL_Y, LARGE_FONT, (Speed / 100U) + '0'));
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_HUNDREDS + 51, SPEED_TEXT_SMALL_Y, LARGE_FONT, ((Speed / 10U) % 10) + '0'));
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_HUNDREDS + 130, SPEED_TEXT_SMALL_Y, LARGE_FONT, (Speed % 10) + '0'));
		}

		else if(Speed >= 10U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_TENS, SPEED_TEXT_Y, LARGE_FONT, (Speed / 10U) + '0'));
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_TENS + 87, SPEED_TEXT_Y, LARGE_FONT, (Speed % 10) + '0'));
		}

		else{
			wr32(RAM_DL + (index+=4), vertex2ii(SPEED_UNITS, SPEED_TEXT_Y, LARGE_FONT, Speed + '0'));
		}
		wr32(RAM_DL + (index+=4), restore_context());


		//Lower Text
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), bitmap_handle(31));
		wr32(RAM_DL + (index+=4), bitmap_transform_a(200));
		wr32(RAM_DL + (index+=4), bitmap_transform_e(256));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), vertex2ii(KM_POSITION_X, KM_POSITION_Y, LARGE_FONT, 'k'));
		wr32(RAM_DL + (index+=4), vertex2ii(KM_POSITION_X + 28, KM_POSITION_Y, LARGE_FONT, 'm'));
		wr32(RAM_DL + (index+=4), vertex2ii(KM_POSITION_X + 73, KM_POSITION_Y, LARGE_FONT, '/'));
		wr32(RAM_DL + (index+=4), vertex2ii(KM_POSITION_X + 94, KM_POSITION_Y, LARGE_FONT, 'h'));

		/* SPEEDOMETER HASH MARKS */
		wr32(RAM_DL + (index+=4), begin(LINES));
		wr32(RAM_DL + (index+=4), line_width(25));

		wr32(RAM_DL + (index+=4), vertex2f(HASH1_X, HASH1_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH1_END_X, HASH1_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH2_X, HASH2_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH2_END_X, HASH2_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH3_X, HASH3_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH3_END_X, HASH3_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH4_X, HASH4_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH4_END_X, HASH4_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH5_X, HASH5_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH5_END_X, HASH5_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH6_X, HASH6_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH6_END_X, HASH6_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH7_X, HASH7_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH7_END_X, HASH7_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH8_X, HASH8_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH8_END_X, HASH8_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH9_X, HASH9_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH9_END_X, HASH9_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH10_X, HASH10_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH10_END_X, HASH10_END_Y));

		wr32(RAM_DL + (index+=4), vertex2f(HASH11_X, HASH11_Y));
		wr32(RAM_DL + (index+=4), vertex2f(HASH11_END_X, HASH11_END_Y));

		wr32(RAM_DL + (index+=4), restore_context());
		/* END FOR SPEEDOMETER HASH MARKS */

		/* DYNAMIC SPEEDOMETER CURSOR */

		double theta = 0;
		double deg = 0;
		deg = 203.0 - ((double)(Speed / 160.0)) * 241.0;
		if(deg < 0){
			deg += 360.0;
		}
		theta = deg * PI / 180.0;




		/* THE END OF SPEEDOMETER */

		/* BATTERY PERCENTAGE BAR */
		wr32(RAM_DL + (index+=4), line_width(16));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(BATTERY_BORDER_X, BATTERY_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(BATTERY_BORDER_X + BATTERY_BORDER_WIDTH, BATTERY_BORDER_Y + BORDER_THICKNESS));
		wr32(RAM_DL + (index+=4), vertex2f(BATTERY_BORDER_X, BATTERY_BORDER_Y + BORDER_THICKNESS));
		wr32(RAM_DL + (index+=4), vertex2f(BATTERY_BORDER_X + BORDER_THICKNESS, BATTERY_BORDER_Y + BATTERY_BORDER_HEIGHT));

		//Calculation for the Battery bar
		Height_Offset = (415 - 60) * (100U - Battery_Percentage) / 100U;
		//Battery percentage fill

		if(Battery_Percentage <= 50U)
		{
			Green = 255U * Battery_Percentage / 50U;
			Red = 255U;
		}
		else if(Battery_Percentage >= 50U)
		{
			Red = 255U * (100 - Battery_Percentage) / 50U;
			Green = 255U;
		}

		wr32(RAM_DL + (index+=4), color_rgb(Red, Green, Blue));
		wr32(RAM_DL + (index+=4), vertex2f(800, BATTERY_BORDER_Y + BATTERY_BORDER_HEIGHT - BORDER_THICKNESS));
		wr32(RAM_DL + (index+=4), vertex2f(BATTERY_BORDER_X + BORDER_THICKNESS + 2, BATTERY_BORDER_Y + BORDER_THICKNESS + Height_Offset));


		//Dynamic Battery percentage
		wr32(RAM_DL + (index+=4), vertex_translate_x(8600));
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		if(Battery_Percentage >= 100U){
			wr32(RAM_DL + (index+=4), vertex2ii(BATTERY_HUNDREDS, BATTERY_TEXT_POSITION_Y, MEDIUM_FONT, (Battery_Percentage / 100U) + '0'));
		}

		if(Battery_Percentage >= 10U){
			wr32(RAM_DL + (index+=4), vertex2ii(BATTERY_TENS, BATTERY_TEXT_POSITION_Y, MEDIUM_FONT, ((Battery_Percentage / 10U) % 10) + '0'));
		}

		wr32(RAM_DL + (index+=4), vertex2ii(BATTERY_UNITS, BATTERY_TEXT_POSITION_Y, MEDIUM_FONT, ((Battery_Percentage % 10) + '0')));
		wr32(RAM_DL + (index+=4), vertex2ii(BATTERY_UNITS + 18, BATTERY_TEXT_POSITION_Y, MEDIUM_FONT, '%'));
		/* THE END FOR BATTERY PERCENTAGE BAR */



		/* MOTOR TEMPERATURE */

		//Bar filling for Motor Temperature
		if(Motor_Temperature <= MOTOR_LIMITED_TEMP){
			Width_Offset = (Motor_Temperature * (800U - 504U)) / 60U;
		}
		else{
			Width_Offset = 296U;
		}

		if(Motor_Temperature <= MOTOR_LIMITED_TEMP){
			Red = (255U * Motor_Temperature) / MOTOR_LIMITED_TEMP;
			Green = (165U * Motor_Temperature) / MOTOR_LIMITED_TEMP;
			Blue = 255U - ((255U * Motor_Temperature) / MOTOR_LIMITED_TEMP);
		}

		else if(Motor_Temperature > MOTOR_LIMITED_TEMP && Motor_Temperature <= 80U){
			Red = 255U;
			Green = 165U - ((165U * (Motor_Temperature - 60U)) / 20U);
			Blue = 0U;
		}

		else{
			Red = 255U;
			Green = 0U;
			Blue = 0U;
		}

		//Dynamic bar filling for Motor
		wr32(RAM_DL + (index+=4), vertex_translate_x(0));
		wr32(RAM_DL + (index+=4), stencil_op(INCR, INCR));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_X, MOTOR_TEMP_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_X - MOTOR_TEMP_WIDTH, MOTOR_TEMP_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_DIAG_X, MOTOR_TEMP_DIAG_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_DIAG_X - MOTOR_TEMP_DIAG_WIDTH, MOTOR_TEMP_DIAG_Y));
		wr32(RAM_DL + (index+=4), stencil_func(GREATER, 0, 255));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), color_rgb(Red, Green, Blue));				//50, 255, 150
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_X, MOTOR_TEMP_DIAG_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_TEMP_X - Width_Offset, MAX_Y));
		wr32(RAM_DL + (index+=4), stencil_func(ALWAYS, 0, 255));

		//Motors Text

		wr32(RAM_DL + (index+=4), vertex_translate_x(8000));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));				//50, 255, 150
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEXT_POSITION_X, MOTOR_TEXT_POSITION_Y, LARGE_FONT, 'M'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEXT_POSITION_X + 33, MOTOR_TEXT_POSITION_Y, LARGE_FONT, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEXT_POSITION_X + 54, MOTOR_TEXT_POSITION_Y, LARGE_FONT, 't'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEXT_POSITION_X + 67, MOTOR_TEXT_POSITION_Y, LARGE_FONT, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEXT_POSITION_X + 90, MOTOR_TEXT_POSITION_Y, LARGE_FONT, 'r'));

		//Dynamic Shadows for Motor Temperature
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));				//50, 255, 150
		if(Motor_Temperature >=10U){
			wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X - 2, MOTOR_TEMP_POSITION_Y + 2, LARGE_FONT, (Motor_Temperature / 10U) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 20, MOTOR_TEMP_POSITION_Y + 2, LARGE_FONT, (Motor_Temperature % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 45, MOTOR_TEMP_POSITION_Y + 2, CELSIUS_SYMBOL, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 52, MOTOR_TEMP_POSITION_Y + 2, LARGE_FONT, 'C'));

		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));				//50, 255, 150
		//Dynamic Text for Motor Temperature
		if(Motor_Temperature >=10U){
			wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X, MOTOR_TEMP_POSITION_Y, LARGE_FONT, (Motor_Temperature / 10U) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 22, MOTOR_TEMP_POSITION_Y, LARGE_FONT, (Motor_Temperature % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 47, MOTOR_TEMP_POSITION_Y, CELSIUS_SYMBOL, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(MOTOR_TEMP_POSITION_X + 54, MOTOR_TEMP_POSITION_Y, LARGE_FONT, 'C'));
		wr32(RAM_DL + (index+=4), vertex_translate_x(0));

		/* THE END FOR MOTOR TEMPERATURE */

		/* INVERTER TEMPERATURE */
		if(Inverter_Temperature <= 60U){
			Width_Offset = (Inverter_Temperature * 297) / 60U;
		}
		else{
			Width_Offset = 297U;
		}

		if(Inverter_Temperature <= 60U){
			Red = (255U * Inverter_Temperature) / 60U;
			Green = (165U * Inverter_Temperature) / 60U;
			Blue = 255U - ((255 * Inverter_Temperature) / 60U);
		}

		else if(Inverter_Temperature > 60U && Inverter_Temperature <= 80U){
			Red = 255U;
			Green = 165U - ((165U * (Inverter_Temperature - 60U)) / 20U);
			Blue = 0U;
		}

		else{
			Red = 255U;
			Green = 0U;
			Blue = 0U;
		}

		//Dynamic Inverter filling
		wr32(RAM_DL + (index+=4), clear(0, 1, 0));
		wr32(RAM_DL + (index+=4), stencil_op(INCR, INCR));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_B));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_TEMP_X, INVERTER_TEMP_Y));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_TEMP_X + INVERTER_TEMP_WIDTH, INVERTER_TEMP_Y));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_TEMP_DIAG_X, INVERTER_TEMP_DIAG_Y));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_TEMP_DIAG_X + INVERTER_TEMP_DIAG_WIDTH, INVERTER_TEMP_DIAG_Y));
		wr32(RAM_DL + (index+=4), stencil_func(GREATER, 0, 255));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), color_rgb(Red, Green, Blue));				//50, 255, 150
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_TEMP_X, INVERTER_TEMP_DIAG_Y - 4));
		wr32(RAM_DL + (index+=4), vertex2f(Width_Offset, MAX_Y));
		wr32(RAM_DL + (index+=4), stencil_func(ALWAYS, 0, 255));

		//Inverter Text
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'I'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 10, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'n'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 32, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'v'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 52, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'e'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 73, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'r'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 88, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 't'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 102, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'e'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEXT_POSITION_X + 122, INVERTER_TEXT_POSITION_Y, LARGE_FONT, 'r'));

		//Dynamic shadows for Inverter temp
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		if(Inverter_Temperature >= 10U){
			wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X - 2, INVERTER_TEMP_POSITION_Y + 2, LARGE_FONT, (Inverter_Temperature / 10) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 20, INVERTER_TEMP_POSITION_Y + 2, LARGE_FONT, (Inverter_Temperature % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 45, INVERTER_TEMP_POSITION_Y + 2, CELSIUS_SYMBOL, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 51, INVERTER_TEMP_POSITION_Y + 2, LARGE_FONT, 'C'));

		//Dynamic temperature for Inverter
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		if(Inverter_Temperature >= 10U){
			wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X, INVERTER_TEMP_POSITION_Y, LARGE_FONT, (Inverter_Temperature / 10) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 22, INVERTER_TEMP_POSITION_Y, LARGE_FONT, (Inverter_Temperature % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 47, INVERTER_TEMP_POSITION_Y, CELSIUS_SYMBOL, 'o'));
		wr32(RAM_DL + (index+=4), vertex2ii(INVERTER_TEMP_POSITION_X + 53, INVERTER_TEMP_POSITION_Y, LARGE_FONT, 'C'));


		/* THE END FOR INVERTER TEMPERATURE */

		/* DYNAMIC ACCELERATION AND BRAKE BAR */

		//Calculation for Dynamic values

		Width_Offset = ((745 - 543) *  Acceleration) / 100U;


		wr32(RAM_DL + (index+=4), stencil_op(INCR, INCR));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_A));
		wr32(RAM_DL + (index+=4), vertex2f(ACCEL_BAR_ORIGIN_X - 1, ACCEL_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), vertex2f(ACCEL_BAR_ORIGIN_X + 152, ACCEL_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), vertex2f(ACCEL_BAR_END_X, 0));
		wr32(RAM_DL + (index+=4), stencil_func(GREATER, 0, 255));
		wr32(RAM_DL + (index+=4), color_rgb(0, 225, 0));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(ACCEL_BAR_ORIGIN_X, ACCEL_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), vertex2f(ACCEL_BAR_ORIGIN_X + Width_Offset, 0));
		wr32(RAM_DL + (index+=4), stencil_func(ALWAYS, 0, 255));
		wr32(RAM_DL + (index+=4), clear(0, 1, 0));

		//Calculation for Dynamic values Brake

		Width_Offset = ((BRAKE_BAR_ORIGIN_X - 58) * Brake) / 100U;

		wr32(RAM_DL + (index+=4), stencil_op(INCR, INCR));
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(EDGE_STRIP_A));
		wr32(RAM_DL + (index+=4), vertex2f(BRAKE_START, 0));
		wr32(RAM_DL + (index+=4), vertex2f(BRAKE_BAR_ORIGIN_X - 149, BRAKE_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), vertex2f(BRAKE_BAR_ORIGIN_X, BRAKE_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), stencil_func(GREATER, 0, 255));
		wr32(RAM_DL + (index+=4), color_rgb(255, 0, 0));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(BRAKE_BAR_ORIGIN_X, BRAKE_BAR_ORIGIN_Y));
		wr32(RAM_DL + (index+=4), vertex2f(BRAKE_BAR_ORIGIN_X - Width_Offset, 0));
		wr32(RAM_DL + (index+=4), stencil_func(ALWAYS, 0, 255));


		/* WARNING LIGHTS SPACE */
		wr32(RAM_DL + (index+=4), color_rgb(80, 80, 80));
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X, WARNING_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X + WARNING_BORDER_WIDTH, WARNING_BORDER_Y + BORDER_THICKNESS));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X, WARNING_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X + BORDER_THICKNESS, WARNING_BORDER_Y - WARNING_BORDER_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X + WARNING_BORDER_WIDTH, WARNING_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(WARNING_BORDER_X + WARNING_BORDER_WIDTH - BORDER_THICKNESS, WARNING_BORDER_Y - WARNING_BORDER_HEIGHT));
		/* END FOR WARNING LIGHTS SPACE */

		//INVERTER_TEMP STATUS
		wr32(RAM_DL + (index+=4), save_context());
		if(Witnesses.Inverter)
		{
			wr32(RAM_DL + (index+=4), color_rgb(250, 120, 0));
		}
		else
		{
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		}
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(TL_INVERTER_CORNER, BOX_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(BR_INVERTER_CORNER, BOX_HEIGHT + BOX_HEIGHT_OFFSET));
		wr32(RAM_DL + (index+=4), restore_context());

		//BATTERY STATUS
		wr32(RAM_DL + (index+=4), save_context());
		if(Witnesses.Battery)
		{
			wr32(RAM_DL + (index+=4), color_rgb(250, 120, 0));
		}
		else
		{
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		}
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(TL_BATTERY_CORNER, BOX_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(BR_BATTERY_CORNER, BOX_HEIGHT + BOX_HEIGHT_OFFSET));
		wr32(RAM_DL + (index+=4), restore_context());

		//ACCELERATION STATUS
		wr32(RAM_DL + (index+=4), save_context());
		if(Witnesses.Acceleration)
		{
			wr32(RAM_DL + (index+=4), color_rgb(250, 120, 0));
		}
		else
		{
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		}
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(TL_ACCEL_CORNER, BOX_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(BR_ACCEL_CORNER, BOX_HEIGHT + BOX_HEIGHT_OFFSET));
		wr32(RAM_DL + (index+=4), restore_context());

		//BRAKE STATUS
		wr32(RAM_DL + (index+=4), save_context());
		if(Witnesses.Brake)
		{
			wr32(RAM_DL + (index+=4), color_rgb(250, 120, 0));
		}
		else
		{
			wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		}
		wr32(RAM_DL + (index+=4), begin(RECTS));
		wr32(RAM_DL + (index+=4), vertex2f(TL_BRAKE_CORNER, BOX_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(BR_BRAKE_CORNER, BOX_HEIGHT + BOX_HEIGHT_OFFSET));
		wr32(RAM_DL + (index+=4), restore_context());


		//Witnesses text
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(0, 0, 0));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));

		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'I'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 5, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'N'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 18, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'V'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 30, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 42, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'R'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 54, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'T'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 64, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 76, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'R'));

		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 101, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'B'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 114, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 124, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'T'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 135, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'T'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 145, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 157, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'R'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 170, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'Y'));

		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 210, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 222, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'C'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 235, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'C'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 247, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X + 259, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'L'));

		wr32(RAM_DL + (index+=4), vertex_translate_x(407*16));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X - 104, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'B'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X - 91, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'R'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X - 78, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X - 67, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'K'));
		wr32(RAM_DL + (index+=4), vertex2ii(WARNING_TEXT_POSITION_X - 55, WARNING_TEXT_POSITION_Y, SMALL_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex_translate_x(0));
		wr32(RAM_DL + (index+=4), restore_context());

		//Text for pedals & time
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(250, 200, 0));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));

		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 145, 1, LARGE_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 170, 1, LARGE_FONT, 'C'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 195, 1, LARGE_FONT, 'C'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 220, 1, LARGE_FONT, 'E'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 245, 1, LARGE_FONT, 'L'));

		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X, 1, LARGE_FONT, 'B'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 25, 1, LARGE_FONT, 'R'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 50, 1, LARGE_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 75, 1, LARGE_FONT, 'K'));
		wr32(RAM_DL + (index+=4), vertex2ii(PEDALS_TEXT_POSITION_X + 100, 1, LARGE_FONT, 'E'));

		wr32(RAM_DL + (index+=4), restore_context());


		/* LOWER BORDERS */
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), line_width(56));
		wr32(RAM_DL + (index+=4), color_rgb(80, 80, 80));
		wr32(RAM_DL + (index+=4), begin(LINE_STRIP));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_HORIZONTAL_BORDER, LOWER_HORIZONTAL_BORDER_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(INVERTER_HORIZONTAL_BORDER + INVERTER_HORIZONTAL_BORDER_WIDTH , LOWER_HORIZONTAL_BORDER_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(MIDDLE_HORIZONTAL_BORDER_X, MIDDLE_HORIZONTAL_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MIDDLE_HORIZONTAL_BORDER_X + MIDDLE_HORIZONTAL_BORDER_WIDTH, MIDDLE_HORIZONTAL_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_HORIZONTAL_BORDER, LOWER_HORIZONTAL_BORDER_HEIGHT));
		wr32(RAM_DL + (index+=4), vertex2f(MOTOR_HORIZONTAL_BORDER + MOTOR_HORIZONTAL_BORDER_WIDTH, LOWER_HORIZONTAL_BORDER_HEIGHT));
		wr32(RAM_DL + (index+=4), line_width(48));
		wr32(RAM_DL + (index+=4), begin(LINES));
		wr32(RAM_DL + (index+=4), vertex2f(LOWER_INVERTER_DELIMITER, MIDDLE_HORIZONTAL_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(LOWER_INVERTER_DELIMITER, MAX_Y));
		wr32(RAM_DL + (index+=4), vertex2f(LOWER_MOTOR_DELIMITER, MIDDLE_HORIZONTAL_BORDER_Y));
		wr32(RAM_DL + (index+=4), vertex2f(LOWER_MOTOR_DELIMITER, MAX_Y));

		/* END FOR LOWER BORDERS DESIGN */

		wr32(RAM_DL + (index+=4), line_width(64));
		//Data underline
		wr32(RAM_DL + (index+=4), begin(LINE_STRIP));
		wr32(RAM_DL + (index+=4), vertex2f(TEMPERATURE_UNDERLINE_X, TEMPERATURE_UNDERLINE_Y));
		wr32(RAM_DL + (index+=4), vertex2f(TEMPERATURE_UNDERLINE_X + TEMPERATURE_UNDERLINE_WIDTH, TEMPERATURE_UNDERLINE_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UNDERLINE_END, TEMPERATURE_UNDERLINE_HEIGHT_OFFSET));
		//Data underline
		wr32(RAM_DL + (index+=4), begin(LINE_STRIP));
		wr32(RAM_DL + (index+=4), vertex2f(VOLTAGE_UNDERLINE_X, VOLTAGE_UNDERLINE_Y));
		wr32(RAM_DL + (index+=4), vertex2f(VOLTAGE_UNDERLINE_X + VOLTAGE_UNDERLINE_WIDTH, VOLTAGE_UNDERLINE_Y));
		wr32(RAM_DL + (index+=4), vertex2f(UNDERLINE_END, VOLTAGE_UNDERLINE_HEIGHT_OFFSET));
		wr32(RAM_DL + (index+=4), restore_context());

		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), bitmap_transform_a(135));
		wr32(RAM_DL + (index+=4), bitmap_transform_e(135));

		/* DYNAMIC VALUES FOR CELL_VOLTAGE */
		if(Cell_Voltage >= 100U){
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, (Cell_Voltage / 100U) + '0'));
		}
		else{
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, '0'));
		}
		if(Cell_Temperature >= 100U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, (Cell_Temperature / 100U) + '0'));
		}

		if(Total_Current >= 100U){
			wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_CURRENT_POSITION_X, TOTAL_CURRENT_POSITION_Y, LARGE_FONT, (Total_Current / 100U) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex_translate_x(8250));
		wr32(RAM_DL + (index+=4), vertex2ii(0, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, '.'));
		if(Cell_Voltage >= 10U){
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X - 455, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, ((Cell_Voltage / 10U) % 10) + '0'));
		}
		else{
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X - 455, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, '0'));
		}

		wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X - 415, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, (Cell_Voltage % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(CELL_VOLTAGE_POSITION_X - 355, CELL_VOLTAGE_POSITION_Y, LARGE_FONT, 'V'));

		if(Cell_Temperature >= 10U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X - 470, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, ((Cell_Temperature / 10) % 10) + '0'));
		}
		else
		{
			wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X - 468, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X - 423, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, '.'));
		wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X - 408, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, (Cell_Temperature % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(CELL_TEMPERATURE_POSITION_X - 338, CELL_TEMPERATURE_POSITION_Y, LARGE_FONT, 'C'));
		if(Total_Current >= 10U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_CURRENT_POSITION_X - 472, TOTAL_CURRENT_POSITION_Y, LARGE_FONT, ((Total_Current / 10) % 10) + '0'));
		}
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_CURRENT_POSITION_X - 425, TOTAL_CURRENT_POSITION_Y, LARGE_FONT, (Total_Current % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_CURRENT_POSITION_X - 370, TOTAL_CURRENT_POSITION_Y, LARGE_FONT, 'A'));
		wr32(RAM_DL + (index+=4), vertex_translate_x(0));
		wr32(RAM_DL + (index+=4), restore_context());


		//Bar color
		/*wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(255, 0, 0));
		wr32(RAM_DL + (index+=4), begin(RECTS));

		wr32(RAM_DL + (index+=4), vertex2f(251, 43));
		wr32(RAM_DL + (index+=4), vertex2f(144, 0));*/



		//Time text
		wr32(RAM_DL + (index+=4), save_context());
		wr32(RAM_DL + (index+=4), color_rgb(250, 200, 0));
		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X, TIME_TEXT_POSITION_Y, LARGE_FONT, ((Minutes / 10) % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 20, TIME_TEXT_POSITION_Y, LARGE_FONT, (Minutes % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 40, TIME_TEXT_POSITION_Y, LARGE_FONT, ':'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 50, TIME_TEXT_POSITION_Y, LARGE_FONT, ((Seconds / 10) % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 72, TIME_TEXT_POSITION_Y, LARGE_FONT, (Seconds % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 95, TIME_TEXT_POSITION_Y, LARGE_FONT, ':'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 106, TIME_TEXT_POSITION_Y, LARGE_FONT, ((Miliseconds / 10) % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TIME_TEXT_POSITION_X + 129, TIME_TEXT_POSITION_Y, LARGE_FONT, (Miliseconds % 10) + '0'));
		wr32(RAM_DL + (index+=4), restore_context());

		wr32(RAM_DL + (index+=4), begin(BITMAPS));
		wr32(RAM_DL + (index+=4), color_rgb(255, 255, 255));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X - 70, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, 'U'));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X - 40, TOTAL_VOLTAGE_POSITION_Y + 13, MEDIUM_FONT, 't'));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X - 25, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, '='));
		if(Total_Voltage >= 100U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, (Total_Voltage / 100) + '0'));
		}
		if(Total_Voltage >= 10U)
		{
			wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X + 22, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, ((Total_Voltage / 10) % 10) + '0'));
		}

		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X + 46, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, (Total_Voltage % 10) + '0'));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X + 71, TOTAL_VOLTAGE_POSITION_Y, LARGE_FONT, 'V'));
		wr32(RAM_DL + (index+=4), vertex_translate_x(8600));
		wr32(RAM_DL + (index+=4), vertex2ii(TOTAL_VOLTAGE_POSITION_X + 28, TOTAL_VOLTAGE_POSITION_Y + 144, MEDIUM_FONT, 'o'));
		wr32(RAM_DL + (index+=4), vertex_translate_x(0));
		wr32(RAM_DL + (index+=4), restore_context());

		wr32(RAM_DL + (index+=4), display());
		wr8(REG_DLSWAP, DLSWAP_FRAME);
	}
}

#ifdef __cplusplus
}
#endif
/** @} */
