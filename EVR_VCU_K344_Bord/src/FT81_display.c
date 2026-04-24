
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


uint32_t alpha_func(AlphaFunc_type func, uint8_t ref){
	return 0;
}

uint32_t begin(Begin_type prim){
	if((prim >= BITMAPS) && (prim <= RECTS)){
		return (((uint32_t)0x1F) << 24) | prim;
	}
	else{
		return 0;
	}
}

uint32_t clear(boolean clear_color, boolean clear_stencil, boolean clear_tag){
	return (((uint32_t)0x26) << 24U) | (((uint32_t)clear_color) << 2U) | (((uint32_t)clear_stencil) << 1U) | ((uint32_t)clear_tag);
}

uint32_t clear_color_rgb(uint8_t Red, uint8_t Green, uint8_t Blue){
	return (((uint32_t)0x02) << 24U) | (((uint32_t)Red) << 16U) | (((uint32_t)Blue) << 8U) | ((uint32_t)Green);
}

uint32_t display(void){
	return 0;
}

uint32_t vertex2f(uint16_t x, uint16_t y){
	return (((uint32_t)0x1) << 30U) | (((uint32_t)x & 0x7FFF) << 15U) | ((uint32_t)y & 0x7FFF);
}

uint32_t vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell){
	return (((uint32_t)0x2) << 30U) | (((uint32_t)(0x1FF & x)) << 21U) | (((uint32_t)(0x1FF & y)) << 12U) | (((uint32_t)(0x1F & handle)) << 7U) | (uint32_t)(0x7F & cell);
}

uint32_t end_d(void){
	return ((uint32_t)0x21) << 24U;
}

uint32_t color_rgb(uint8_t red, uint8_t green, uint8_t blue){
	return (((uint32_t)0x04) << 24U) | ((uint32_t)red << 16U) | ((uint32_t)green << 8U) | (uint32_t)blue;
}

uint32_t point_size(uint16_t size){
	return (((uint32_t)0x0D) << 24U) | (uint32_t)(size & 0x1FFF);
}

uint32_t vertex_format(uint8_t frac){
	if (frac > 4){
		frac = 4;
	}
	return (((uint32_t)0x27) << 24U) | (frac & 0x07);
}

uint32_t vertex_translate_x(uint32_t x){
	return (((uint32_t)0x2B) << 24U) | (x & 0x1FFFF);
}

uint32_t vertex_translate_y(uint32_t y){
	return (((uint32_t)0x2C) << 24U) | (y & 0x1FFFF);
}

uint32_t bitmap_source(uint32_t addr){
	return (0x01 << 24U) | (addr & 0x3FFFFF);
}

uint32_t bitmap_layout(BitmapLayout_type format, uint16_t linestride, uint16_t height){
	return (0x07 << 24U) | ((format & 0x1F) << 19) | ((linestride & 0x3FF) << 9) | (height & 0x1FF);
}

uint32_t bitmap_size(boolean filter, boolean wrapX, boolean wrapY, uint16_t width, uint16_t height){
	return (0x08 << 24) | ((filter & 0x1) << 20) | ((wrapX & 0x1) << 19) | ((wrapY & 0x1) << 18) | ((width & 0x1FF) << 9) | (height & 0x1FF);
}

uint32_t bitmap_transform_a(uint32_t coefficient){
	return (0x15 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_transform_b(uint32_t coefficient){
	return (0x16 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_transform_c(uint32_t coefficient){
	return (0x17 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_transform_d(uint32_t coefficient){
	return (0x18 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_transform_e(uint32_t coefficient){
	return (0x19 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_transform_f(uint32_t coefficient){
	return (0x20 << 24) | (coefficient & 0x1FFFF);
}

uint32_t bitmap_handle(uint8_t handle){
	return (0x05 << 24) | (handle & 0x1F);
}

uint32_t line_width(uint16_t width){
return (0x0E << 24) | (width & 0x7FF);
}

uint32_t save_context(void){
	return 0x22 << 24;
}

uint32_t restore_context(void){
	return 0x23 << 24;
}

uint32_t stencil_func(AlphaFunc_type func, uint8_t ref, uint8_t mask){
	return (0x0A << 24) | (func << 16) | (ref << 8) | mask;
}

uint32_t clear_stencil(void){
	return 0x11 << 24;
}

uint32_t stencil_op(StencilOp_t sfail, StencilOp_t spass){
	return (0x0C << 24) | (sfail << 3) | spass;
}


#ifdef __cplusplus
}
#endif
/** @} */
