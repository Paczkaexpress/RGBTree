#include "stm32f4xx.h"
#include "cmsis_lib/include/stm32f4xx_gpio.h"

/******************************************************
 * General definitions
 ******************************************************/

#define TRUE	1
#define FALSE   0

#define ON		1
#define OFF   	0

#define RED 	0
#define GREEN 	1
#define BLUE	2

typedef uint8_t bool;
typedef uint8_t c_color;

enum LightModes{
	HSVtoRGBwithDelay = 1,
	HSVtoRGB		  = 2,
	FourColorTest     = 3,
	DummyRGBwithPWM   = 4,
	FourSeasons		  = 5,
	Strobo			  = 6,
	SevenColorMode    = 7,
	Random			  = 8,

};

/******************************************************
 * Pins names definitions
 ******************************************************/

/******************************************************
 * Functions prototypes
 ******************************************************/

/******************************************************
 * Structure prototypes
 ******************************************************/

typedef struct _RedColor
{
		uint16_t CurrnetValue[64];
		uint16_t DestinationValue[64];
		uint16_t ChangeSpeed[64];
}RedColor;

typedef struct _GreenColor
{
		uint16_t CurrnetValue[64];
		uint16_t DestinationValue[64];
		uint16_t ChangeSpeed[64];
}GreenColor;


typedef struct _BlueColor
{
		uint32_t CurrnetValue[64];
		uint32_t DestinationValue[64];
		uint32_t ChangeSpeed[64];
}BlueColor;

typedef struct _HSVHue
{
		uint32_t CurrnetValue[64];
		uint32_t DestinationValue[64];
		uint32_t ChangeSpeed[64];
}HSVHue;

typedef struct _HSVSaturation
{
		uint32_t CurrnetValue[64];
		uint32_t DestinationValue[64];
		uint32_t ChangeSpeed[64];
}HSVSaturation;

typedef struct _HSVValue
{
		uint16_t CurrnetValue[64];
		uint16_t DestinationValue[64];
		uint16_t ChangeSpeed[64];
}HSVValue;

typedef struct _ProgramOptions
{
		bool OnOff;
		uint8_t Mode;

}ProgramOptions;
