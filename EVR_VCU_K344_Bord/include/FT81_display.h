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
uint32 alpha_func(AlphaFunc_type func, uint8 ref);
uint32 bitmap_handle(uint8 handle);
uint32 bitmap_layout(BitmapLayout_type format, uint16 linestride, uint16 height);
//uint32 bitmap_layout_h();
uint32 bitmap_size(boolean filter, boolean wrapX, boolean wrapY, uint16 width, uint16 height);
//uint32 bitmap_size_h();
uint32 bitmap_source(uint32 addr);
uint32 bitmap_transform_a(uint32 coefficient);
uint32 bitmap_transform_b(uint32 coefficient);
uint32 bitmap_transform_c(uint32 coefficient);
uint32 bitmap_transform_d(uint32 coefficient);
uint32 bitmap_transform_e(uint32 coefficient);
uint32 bitmap_transform_f(uint32 coefficient);

//uint32 blend_func();
//uint32 cell();
uint32 clear(boolean clear_color, boolean clear_stencil, boolean clear_tag);
//uint32 clear_color_a();
uint32 clear_color_rgb(uint8 Red, uint8 Green,  uint8 Blue);
//uint32 clear_stencil();
//uint32 clear_tag();
//uint32 color_a();
//uint32 color_mask();
uint32 color_rgb(uint8 red, uint8 green, uint8 blue);
uint32 line_width(uint16 width);
uint32 point_size(uint16 size);
uint32 restore_context(void);
uint32 save_context(void);
//uint32 scissor_size();
//uint32 scissor_xy();
//uint32 stencil_func();
//uint32 stencil_mask();
//uint32 stencil_op();
//uint32 tag();
//uint32 tag_mask();
uint32 vertex_format(uint8 frac);
uint32 vertex_translate_x(uint32 x);
uint32 vertex_translate_y(uint32 y);
//uint32 palette_source();
/*Drawing actions*/
uint32 begin(Begin_type prim);
uint32 end_d(void);
uint32 vertex2f(uint16 x, uint16 y);
uint32 vertex2ii(uint16 x, uint16 y, uint8 handle, uint8 cell);
/*Execution control*/
//uint32 nop();
//uint32 jump();
//uint32 macro();
//uint32 call();
//uint32 return();
uint32 display(void);


#ifdef __cplusplus
}
#endif
/** @} */
#endif
