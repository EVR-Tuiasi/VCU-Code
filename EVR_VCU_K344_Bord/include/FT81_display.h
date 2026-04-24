#ifndef FT81_DISPLAY_H
#define FT81_DISPLAY_H

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
/*Graphics Engine Registers*/
#define REG_PCLK 		(RAM_REG + 0x70)
#define REG_PCLK_POL	(RAM_REG + 0x6C)
#define REG_CSPREAD		(RAM_REG + 0x68)
#define REG_DITHER		(RAM_REG + 0x60)
#define REG_SWIZZLE 	(RAM_REG + 0x64)
#define REG_OUTBITS		(RAM_REG + 0x5C)
#define REG_ROTATE		(RAM_REG + 0x58)
#define REG_VSYNC1		(RAM_REG + 0x50)
#define REG_VSYNC0		(RAM_REG + 0x4C)
#define REG_VSIZE		(RAM_REG + 0x48)
#define REG_VOFFSET		(RAM_REG + 0x44)
#define REG_VCYCLE		(RAM_REG + 0x40)
#define REG_HSYNC1		(RAM_REG + 0x3C)
#define REG_HSYNC0		(RAM_REG + 0x38)
#define REG_HSIZE		(RAM_REG + 0x34)
#define REG_HOFFSET		(RAM_REG + 0x30)
#define REG_HCYCLE		(RAM_REG + 0x2C)
#define REG_DLSWAP		(RAM_REG + 0x54)
#define REG_TAG			(RAM_REG + 0x7C)
#define REG_TAG_Y		(RAM_REG + 0x78)
#define REG_TAG_X		(RAM_REG + 0x74)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
typedef enum{
	NEVER,
	LESS,
	LEQUAL,
	GREATER,
	GEQUAL,
	EQUAL,
	NOTEQUAL,
	ALWAYS
}AlphaFunc_type;

typedef enum{
	BEGIN_TYPE_RESERVED,
	BITMAPS,
	POINTS,
	LINES,
	LINE_STRIP,
	EDGE_STRIP_R,
	EDGE_STRIP_L,
	EDGE_STRIP_A,
	EDGE_STRIP_B,
	RECTS
}Begin_type;

typedef enum{
	ARGB1555,
	L1,
	L4,
	L8,
	RGB332,
	ARGB2,
	ARGB4,
	RGB565,
	BITMAP_LAYOUT_RESERVED,
	TEXT8X8,
	TEXTVGA,
	BARGRAPH,
	BITMAP_LAYOUT_RESERVED_2,
	BITMAP_LAYOUT_RESERVED_3,
	PALETTED565,
	PALETTED4444,
	PALETTED8,
	L2
}BitmapLayout_type;

typedef enum{
	ZERO,
	KEEP,
	REPLACE,
	INCR,
	DECR,
	INVERT
}StencilOp_t;
/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
/*Setting graphics state*/
uint32_t alpha_func(AlphaFunc_type func, uint8_t ref);
uint32_t bitmap_handle(uint8_t handle);
uint32_t bitmap_layout(BitmapLayout_type format, uint16_t linestride, uint16_t height);
//uint32_t bitmap_layout_h();
uint32_t bitmap_size(boolean filter, boolean wrapX, boolean wrapY, uint16_t width, uint16_t height);
//uint32_t bitmap_size_h();
uint32_t bitmap_source(uint32_t addr);
uint32_t bitmap_transform_a(uint32_t coefficient);
uint32_t bitmap_transform_b(uint32_t coefficient);
uint32_t bitmap_transform_c(uint32_t coefficient);
uint32_t bitmap_transform_d(uint32_t coefficient);
uint32_t bitmap_transform_e(uint32_t coefficient);
uint32_t bitmap_transform_f(uint32_t coefficient);

//uint32_t blend_func();
//uint32_t cell();
uint32_t clear(boolean clear_color, boolean clear_stencil, boolean clear_tag);
//uint32_t clear_color_a();
uint32_t clear_color_rgb(uint8_t Red, uint8_t Green,  uint8_t Blue);
//uint32_t clear_stencil();
//uint32_t clear_tag();
//uint32_t color_a();
//uint32_t color_mask();
uint32_t color_rgb(uint8_t red, uint8_t green, uint8_t blue);
uint32_t line_width(uint16_t width);
uint32_t point_size(uint16_t size);
uint32_t restore_context(void);
uint32_t save_context(void);
//uint32_t scissor_size();
//uint32_t scissor_xy();
//uint32_t stencil_func();
//uint32_t stencil_mask();
//uint32_t stencil_op();
//uint32_t tag();
//uint32_t tag_mask();
uint32_t vertex_format(uint8_t frac);
uint32_t vertex_translate_x(uint32_t x);
uint32_t vertex_translate_y(uint32_t y);
//uint32_t palette_source();
/*Drawing actions*/
uint32_t begin(Begin_type prim);
uint32_t end_d(void);
uint32_t vertex2f(uint16_t x, uint16_t y);
uint32_t vertex2ii(uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
/*Execution control*/
//uint32_t nop();
//uint32_t jump();
//uint32_t macro();
//uint32_t call();
//uint32_t return();
uint32_t display(void);
uint32_t stencil_func(AlphaFunc_type func, uint8_t ref, uint8_t mask);
uint32_t stencil_op(StencilOp_t sfail, StencilOp_t spass);


#ifdef __cplusplus
}
#endif
/** @} */
#endif
