#ifndef FT81_SOUND_H
#define FT81_SOUND_H

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
/*Audio Engine Registers*/
#define REG_PLAY				(RAM_REG + 0x8C)
#define REG_SOUND				(RAM_REG + 0x88)
#define REG_VOL_SOUND			(RAM_REG + 0x84)
#define REG_VOL_PB				(RAM_REG + 0x80)
#define REG_PLAYBCK_PLAY		(RAM_REG + 0xCC)
#define REG_PLAYBACK_LOOP		(RAM_REG + 0xC8)
#define REG_PLAYBACK_FORMAT		(RAM_REG + 0xC4)
#define REG_PLAYBACK_FREQ		(RAM_REG + 0xC0)
#define REG_PLAYBACK_READPTR	(RAM_REG + 0xBC)
#define REG_PLAYBACK_LENGTH		(RAM_REG + 0xB8)
#define REG_PLAYBACK_START		(RAM_REG + 0xB4)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
typedef enum{
	LINEAR_SAMPLE,
	ULAW_SAMPLE,
	IMA_ADPCM_SAMPLE
}SoundFormat_type;

typedef enum{
	SILENCE = 0x00,
	SQUARE_WAVE = 0x01,
	SINE_WAVE = 0x02,
	SAWTOOTH_WAVE = 0x03,
	TRIANGLE_WAVE = 0x04,
	BEEPING = 0x05,
	ALARM = 0x06,
	WARBLE = 0x07,
	CAROUSEL = 0x08,
	ONE_SHORT_PIP = 0x10,
	TWO_SHORT_PIPS = 0x11,
	THREE_SHORT_PIPS = 0x12,
	FOUR_SHORT_PIPS = 0x13,
	FIVE_SHORT_PIPS = 0x14,
	SIX_SHORT_PIPS = 0x15,
	SEVEN_SHORT_PIPS = 0x16,
	EIGHT_SHORT_PIPS = 0x17,
	NINE_SHORT_PIPS = 0x18,
	TEN_SHORT_PIPS = 0x19,
	ELEVEN_SHORT_PIPS = 0x1A,
	TWELVE_SHORT_PIPS = 0x1B,
	THIRTEEN_SHORT_PIPS = 0x1C,
	FOURTEEN_SHORT_PIPS = 0x1D,
	FIFTEEN_SHORT_PIPS = 0x1E,
	SIXTEEN_SHORT_PIPS = 0x1F,
	DTFM_HASHTAG = 0x23,
	DTFM_ASTERISK = 0x2C,
	DTFM_ZERO = 0x30,
	DTFM_ONE = 0x31,
	DTFM_TWO = 0x32,
	DTFM_THREE = 0x33,
	DTFM_FOUR = 0x34,
	DTFM_FIVE = 0x35,
	DTFM_SIX = 0x36,
	DTFM_SEVEN = 0x37,
	DTFM_EIGHT = 0x38,
	DTFM_NINE = 0x39,
	HARP = 0x40,
	XYLOPHONE = 0x41,
	TUBA = 0x42,
	GLOCKENSPIEL = 0x43,
	ORGAN = 0x44,
	TRUMPET = 0x45,
	PIANO = 0x46,
	CHIMES = 0x47,
	MUSIC_BOX = 0x48,
	BELL = 0x49,
	CLICK = 0x50,
	SWITCH = 0x51,
	COWBELL = 0x52,
	NOTCH = 0x53,
	HIHAT = 0x54,
	KICKDRUM = 0x55,
	POP = 0x56,
	CLACK = 0x57,
	CHACK = 0x58,
	MUTE = 0x60,
	UNMUTE = 0x61
}SoundEffect_type;

typedef enum{
	A0,
	As0,
	B0,
	B00
}MidiEffect_type;
/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
uint32 effect_start(void);
uint32 effect_select(SoundEffect_type effect, uint8 pitch);
uint32 effect_volume(uint8 sound_level);
uint32 file_volume(uint8 sound_level);
uint32 file_playback_start(void);
uint32 file_playback_loop(boolean enable);
uint32 file_playback_format(SoundFormat_type format);
uint32 file_playback_sampling_frequency(uint16 frequency);
uint32 file_playback_length(uint32 length);
uint32 file_playback_start_address(uint32 address);

#ifdef __cplusplus
}
#endif

#endif
