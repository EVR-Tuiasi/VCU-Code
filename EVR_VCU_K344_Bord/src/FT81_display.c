
#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mcu.h"
#include "FT81_display.h"

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


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


uint32 alpha_func(AlphaFunc_type func, uint8 ref){
	return 0;
}

uint32 begin(Begin_type prim){
	if((prim >= BITMAPS) && (prim <= RECTS)){
		return (((uint32)0x1F) << 24) | prim;
	}
	else{
		return 0;
	}
}

uint32 clear(boolean clear_color, boolean clear_stencil, boolean clear_tag){
	return (((uint32)0x26) << 24U) | (((uint32)clear_color) << 2U) | (((uint32)clear_stencil) << 1U) | ((uint32)clear_tag);
}

uint32 clear_color_rgb(uint8 Red, uint8 Green, uint8 Blue){
	return (((uint32)0x02) << 24U) | (((uint32)Red) << 16U) | (((uint32)Blue) << 8U) | ((uint32)Green);
}

uint32 display(void){
	return 0;
}

uint32 vertex2f(uint16 x, uint16 y){
	return (((uint32)0x1) << 30U) | (((uint32)x & 0x7FFF) << 15U) | ((uint32)y & 0x7FFF);
}

uint32 vertex2ii(uint16 x, uint16 y, uint8 handle, uint8 cell){
	return (((uint32)0x2) << 30U) | (((uint32)(0x1FF & x)) << 21U) | (((uint32)(0x1FF & y)) << 12U) | (((uint32)(0x1F & handle)) << 7U) | (uint32)(0x7F & cell);
}

uint32 end_d(void){
	return ((uint32)0x21) << 24U;
}

uint32 color_rgb(uint8 red, uint8 green, uint8 blue){
	return (((uint32)0x04) << 24U) | ((uint32)red << 16U) | ((uint32)green << 8U) | (uint32)blue;
}

uint32 point_size(uint16 size){
	return (((uint32)0x0D) << 24U) | (uint32)(size & 0x1FFF);
}

uint32 vertex_format(uint8 frac){
	if (frac > 4){
		frac = 4;
	}
	return (((uint32)0x27) << 24U) | (frac & 0x07);
}

uint32 vertex_translate_x(uint32 x){
	return (((uint32)0x2B) << 24U) | (x & 0x1FFFF);
}

uint32 vertex_translate_y(uint32 y){
	return (((uint32)0x2C) << 24U) | (y & 0x1FFFF);
}

uint32 bitmap_source(uint32 addr){
	return (0x01 << 24U) | (addr & 0x3FFFFF);
}

uint32 bitmap_layout(BitmapLayout_type format, uint16 linestride, uint16 height){
	return (0x07 << 24U) | ((format & 0x1F) << 19) | ((linestride & 0x3FF) << 9) | (height & 0x1FF);
}

uint32 bitmap_size(boolean filter, boolean wrapX, boolean wrapY, uint16 width, uint16 height){
	return (0x08 << 24) | ((filter & 0x1) << 20) | ((wrapX & 0x1) << 19) | ((wrapY & 0x1) << 18) | ((width & 0x1FF) << 9) | (height & 0x1FF);
}

uint32 bitmap_transform_a(uint32 coefficient){
	return (0x15 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_transform_b(uint32 coefficient){
	return (0x16 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_transform_c(uint32 coefficient){
	return (0x17 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_transform_d(uint32 coefficient){
	return (0x18 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_transform_e(uint32 coefficient){
	return (0x19 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_transform_f(uint32 coefficient){
	return (0x20 << 24) | (coefficient & 0x1FFFF);
}

uint32 bitmap_handle(uint8 handle){
	return (0x05 << 24) | (handle & 0x1F);
}

uint32 line_width(uint16 width){
return (0x0E << 24) | (width & 0x7FF);
}

uint32 save_context(void){
	return 0x22 << 24;
}

uint32 restore_context(void){
	return 0x23 << 24;
}


#ifdef __cplusplus
}
#endif
/** @} */
