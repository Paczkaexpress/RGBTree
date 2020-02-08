#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h."
#include <assert.h>

#include"stm32f4xx_tim.h"
#include"main.h"
#include"LedDriver.h"

extern RedColor redColor;
extern GreenColor greenColor;
extern BlueColor blueColor;
extern HSVHue	hsvHue;
extern HSVSaturation hsvSaturation;
extern HSVValue hsvValue;

extern pwmTimer;

void HSV_to_RGB()
{
	int i = 0;

	long int X = 0;
	long int m = 0;
	long int C = 0;

	double dX;
	for(i=0;i<64;i++)
	{
		C = hsvSaturation.CurrnetValue[i] * hsvValue.CurrnetValue[i];
		X = 5 * (6000 - abs((hsvHue.CurrnetValue[i]*100) % 12000 - 6000)) / 3;
		m = hsvValue.CurrnetValue[i]*100 - C;

		if(hsvHue.CurrnetValue[i] < 60)
		{
			redColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
			greenColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
			blueColor.CurrnetValue[i] = 0;
		}
		else if(hsvHue.CurrnetValue[i] < 120)
		{
			redColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
			greenColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
			blueColor.CurrnetValue[i] = 0;
		}
		else if(hsvHue.CurrnetValue[i] < 180)
		{
			redColor.CurrnetValue[i] = 0;
			greenColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
			blueColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
		}
		else if(hsvHue.CurrnetValue[i] < 240)
		{
			redColor.CurrnetValue[i] = 0;
			greenColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
			blueColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
		}
		else if(hsvHue.CurrnetValue[i] < 300)
		{
			redColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
			greenColor.CurrnetValue[i] = 0;
			blueColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
		}
		else if(hsvHue.CurrnetValue[i] < 360)
		{
			redColor.CurrnetValue[i] = ((C+m) * 255) / 10000;
			greenColor.CurrnetValue[i] = 0;
			blueColor.CurrnetValue[i] = ((X+m) * 255) / 10000;
		}
	}
}

inline void redLedDriverTest(volatile uint64_t data)
{
	int i=0;
	for(i=0;i<64;i++)
	{
		LED_RED_CLK_CLR();

		if(data & 1)
		{
			LED_RED_DATA_SET();
		}
		else
		{
			LED_RED_DATA_CLR();
		}

		data = data>>1;
		LED_RED_CLK_SET();
		asm("nop");
	}

	LED_RED_LA_SET();
	asm("nop");
	LED_RED_LA_CLR();
}

inline void blueLedDriverTest(volatile uint64_t data) //tak na prawde jest to niebieski
{
	int i=0;
	for(i=0;i<64;i++)
	{
		GPIOC->BSRRH = 256; //wstawianie wartosci bezposrenio do rejestru

		if(data & ((uint64_t)1<<i))
		{
			//LED_GREEN_DATA_SET(); //GPIOx->BSRRL = GPIO_Pin; 512
			GPIOC->BSRRL = 512;
		}
		else
		{
			//LED_GREEN_DATA_CLR(); //GPIOx->BSRRH = GPIO_Pin;
			GPIOC->BSRRH = 512;
		}

		GPIOC->BSRRL = 256;

	}

	LED_GREEN_LA_SET();
	asm("nop");
	LED_GREEN_LA_CLR();
}

void greenLedDriverTest(volatile uint64_t data)
{

	int i=0;
	for(i=0;i<64;i++)
	{
		LED_BLUE_CLK_CLR();

		if(data & 1)
		{
			LED_BLUE_DATA_SET();
		}
		else
		{
			LED_BLUE_DATA_CLR();
		}

		data = data>>1;
		LED_BLUE_CLK_SET();
		asm("nop");
	}

	LED_BLUE_LA_SET();
	asm("nop");
	LED_BLUE_LA_CLR();
}

inline void redLedDriver()
{
	int i=0;
	for(i=0;i<64;i++)
	{
		LED_RED_CLK_CLR();

		if((uint16_t)redColor.CurrnetValue[i] > (uint16_t)pwmTimer)
		{
			LED_RED_DATA_SET();
		}
		else
		{
			LED_RED_DATA_CLR();
		}
		LED_RED_CLK_SET();
		asm("nop");
	}

	LED_RED_LA_SET();
	asm("nop");
	LED_RED_LA_CLR();
}

inline void blueLedDriver() //tak na prawde jest to niebieski
{
	int i=0;
	for(i=0;i<64;i++)
	{
		GPIOC->BSRRH = 256; //wstawianie wartosci bezposrenio do rejestru

		if((uint16_t)blueColor.CurrnetValue[i] > (uint16_t)pwmTimer)
		{
			//LED_GREEN_DATA_SET(); //GPIOx->BSRRL = GPIO_Pin; 512
			GPIOC->BSRRL = 512;
		}
		else
		{
			//LED_GREEN_DATA_CLR(); //GPIOx->BSRRH = GPIO_Pin;
			GPIOC->BSRRH = 512;
		}

		GPIOC->BSRRL = 256;

	}

	LED_GREEN_LA_SET();
	asm("nop");
	LED_GREEN_LA_CLR();
}

void greenLedDriver()
{

	int i=0;
	for(i=0;i<64;i++)
	{
		LED_BLUE_CLK_CLR();

		if((uint16_t)greenColor.CurrnetValue[i] > (uint16_t)pwmTimer)
		{
			LED_BLUE_DATA_SET();
		}
		else
		{
			LED_BLUE_DATA_CLR();
		}

		LED_BLUE_CLK_SET();
		asm("nop");
	}

	LED_BLUE_LA_SET();
	asm("nop");
	LED_BLUE_LA_CLR();
}


void LedDriverTest()
{
	greenLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(1000);
	greenLedDriverTest(0);
	redLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(1000);
	redLedDriverTest(0);
	blueLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(1000);
	blueLedDriverTest(0);
	return 0;

}

void LedDummyPWMTest()
{
	int i = 0;
	int j = 0;
	for(i=0;i<100;i++)
	{
		for(j=0;j<2;j++)
		{
			greenLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			greenLedDriverTest(0);
			redLedDriverTest(0);
			blueLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}
	for(i=100;i>0;i--)
	{
		for(j=0;j<2;j++)
		{
			greenLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			greenLedDriverTest(0);
			redLedDriverTest(0);
			blueLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}

	for(i=0;i<100;i++)
	{
		for(j=0;j<2;j++)
		{
			redLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			redLedDriverTest(0);
			greenLedDriverTest(0);
			blueLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}
	for(i=100;i>0;i--)
	{
		for(j=0;j<2;j++)
		{
			redLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			redLedDriverTest(0);
			greenLedDriverTest(0);
			blueLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}

	for(i=0;i<100;i++)
	{
		for(j=0;j<2;j++)
		{
			blueLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			blueLedDriverTest(0);
			redLedDriverTest(0);
			greenLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}
	for(i=100;i>0;i--)
	{
		for(j=0;j<2;j++)
		{
			blueLedDriverTest(0xFFffFFffFFffFFff);
			delay_us(i*100);
			blueLedDriverTest(0);
			redLedDriverTest(0);
			greenLedDriverTest(0);
			delay_us(10000-i*100);
		}
	}


}

void LedSevenColorMode()
{
	int i;
	uint64_t redValue = 0;
	uint64_t greenValue = 0;
	uint64_t blueValue = 0;

	while(1)
	for(i = 0;i<64;i++)
	{
    	greenLedDriverTest(greenValue);
    	redLedDriverTest(redValue);
    	blueLedDriverTest(blueValue);
    	delay_ms(200);
    	redValue = redValue << 1;
    	greenValue = greenValue << 1;
    	blueValue = blueValue << 1;
    	if(i%3 == 0 || i%5 == 0)
    	{
    		redValue |= 1;
    	}
    	if((i+1)%3 == 0 || (i+1)%5 == 0)
    	{
    		greenValue |= 1;
    	}
    	if((i+2)%3 == 0 || (i+2)%5 == 0)
    	{
    		blueValue |= 1;
    	}
	}

	return 0;
}

void LedStrobo()
{
	blueLedDriverTest(0);
	redLedDriverTest(0);
	greenLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(70);

	blueLedDriverTest(0);
	greenLedDriverTest(0);
	redLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(70);

	blueLedDriverTest(0xFFffFFffFFffFFff);
	greenLedDriverTest(0);
	redLedDriverTest(0);
	delay_ms(70);

	blueLedDriverTest(0);
	greenLedDriverTest(0xFFffFFffFFffFFff);
	redLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(70);

	blueLedDriverTest(0xFFffFFffFFffFFff);
	greenLedDriverTest(0);
	redLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(70);

	blueLedDriverTest(0xFFffFFffFFffFFff);
	greenLedDriverTest(0xFFffFFffFFffFFff);
	redLedDriverTest(0);
	delay_ms(70);

	blueLedDriverTest(0xFFffFFffFFffFFff);
	greenLedDriverTest(0xFFffFFffFFffFFff);
	redLedDriverTest(0xFFffFFffFFffFFff);
	delay_ms(70);

	return 0;
}

void LedRandom() {
	int i;
	uint64_t redValue = 0;
	uint64_t greenValue = 0;
	uint64_t blueValue = 0;

	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET) {
		RNG_ClearFlag(RNG_FLAG_SECS);
		RNG_ClearFlag(RNG_FLAG_CECS);
		RNG_DeInit();
		RNG_Cmd(ENABLE);
	}

	//greenLedDriverTest((uint64_t)RNG_GetRandomNumber()<<32 + RNG_GetRandomNumber());
	greenLedDriverTest((uint64_t) 1 << (RNG_GetRandomNumber() % 64));
	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET) {
		RNG_ClearFlag(RNG_FLAG_SECS);
		RNG_ClearFlag(RNG_FLAG_CECS);
		RNG_DeInit();
		RNG_Cmd(ENABLE);
	}

	//redLedDriverTest((uint64_t)RNG_GetRandomNumber()<<32 + RNG_GetRandomNumber());
	redLedDriverTest((uint64_t) 1 << (RNG_GetRandomNumber() % 64));
	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET) {
		RNG_ClearFlag(RNG_FLAG_SECS);
		RNG_ClearFlag(RNG_FLAG_CECS);
		RNG_DeInit();
		RNG_Cmd(ENABLE);
	}

	//blueLedDriverTest((uint64_t)RNG_GetRandomNumber()<<32 + RNG_GetRandomNumber());
	blueLedDriverTest((uint64_t) 1 << (RNG_GetRandomNumber() % 64));
	delay_ms(100);

}

void LedTurnOnSegment(int SegmentNumber, c_color color)
{
	assert(SegmentNumber <= 7 && SegmentNumber >=0);
	assert(color == RED || color == GREEN || color == BLUE);

	int i;

	if(color == RED)
	{
		for(i = 0;i < 8; i++)
		{
			redColor.CurrnetValue[SegmentNumber*8 + i] = 255;
		}

		redLedDriver();
	}
	else if(color == GREEN)
	{
		for(i = 0;i < 8; i++)
		{
			greenColor.CurrnetValue[SegmentNumber*8 + i] = 255;
		}

		greenLedDriver();
	}
	else if(color == BLUE)
	{
		for(i = 0;i < 8; i++)
		{
			blueColor.CurrnetValue[SegmentNumber*8 + i] = 255;
		}

		blueLedDriver();
	}
}

void LedTurnOffSegment(int SegmentNumber, c_color color)
{
	assert(SegmentNumber <= 7 && SegmentNumber >=0);
	assert(color == RED || color == GREEN || color == BLUE);

	int i;

	if(color == RED)
	{
		for(i = 0;i < 8; i++)
		{
			redColor.CurrnetValue[SegmentNumber*8 + i] = 0;
		}

		redLedDriver();
	}
	else if(color == GREEN)
	{
		for(i = 0;i < 8; i++)
		{
			greenColor.CurrnetValue[SegmentNumber*8 + i] = 0;
		}

		greenLedDriver();
	}
	else if(color == BLUE)
	{
		for(i = 0;i < 8; i++)
		{
			blueColor.CurrnetValue[SegmentNumber*8 + i] = 0;
		}

		blueLedDriver();
	}
}


/**
  * @brief  Jakas funkcja
  * @note   Funkcja losujaca ktora dioda ma siê zapalic, nastepnie z kazdym cyklem
  * funkcji jest zmiejszana wartos jasnosci tej diody
  * @retval None
  */
void LedRandomSinglePWM() //nie wiem jak to nazwaæ wiêc niech mo¿e na razie tak bêdzie :P
{
    while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET)
    {
    	RNG_ClearFlag(RNG_FLAG_SECS);
    	RNG_ClearFlag(RNG_FLAG_CECS);
    	RNG_DeInit();
    	RNG_Cmd(ENABLE);
    	delay_us(1);
    }
	uint64_t Led  = RNG_GetRandomNumber() % 64;

	redColor.CurrnetValue[Led] = 255;

    while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET)
    {
    	RNG_ClearFlag(RNG_FLAG_SECS);
    	RNG_ClearFlag(RNG_FLAG_CECS);
    	RNG_DeInit();
    	RNG_Cmd(ENABLE);
    	delay_us(1);
    }
	Led  = RNG_GetRandomNumber() % 64;

	greenColor.CurrnetValue[Led] = 255;

    while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET)
    {
    	RNG_ClearFlag(RNG_FLAG_SECS);
    	RNG_ClearFlag(RNG_FLAG_CECS);
    	RNG_DeInit();
    	RNG_Cmd(ENABLE);
    	delay_us(1);
    }
	Led  = RNG_GetRandomNumber() % 64;

	blueColor.CurrnetValue[Led] = 255;
	//UpdateLedValues();

}

void UpdateLedValues()
{
	int i;
	for(i = 0;i<64;i++)
	{
		if(redColor.CurrnetValue[i] > redColor.DestinationValue[i])
		{
			redColor.CurrnetValue[i] = redColor.CurrnetValue[i] - redColor.ChangeSpeed[i];
		}
		else if(redColor.CurrnetValue[i] < redColor.DestinationValue[i])
		{
			redColor.CurrnetValue[i] = redColor.CurrnetValue[i] + redColor.ChangeSpeed[i];
		}
		else
		{

		}
	}

	for(i = 0;i<64;i++)
	{
		if(greenColor.CurrnetValue[i] > greenColor.DestinationValue[i])
		{
			greenColor.CurrnetValue[i] = greenColor.CurrnetValue[i] - greenColor.ChangeSpeed[i];
		}
		else if(greenColor.CurrnetValue[i] < greenColor.DestinationValue[i])
		{
			greenColor.CurrnetValue[i] = greenColor.CurrnetValue[i] + greenColor.ChangeSpeed[i];
		}
		else
		{

		}
	}

	for(i = 0;i<64;i++)
	{
		if(blueColor.CurrnetValue[i] > blueColor.DestinationValue[i])
		{
			blueColor.CurrnetValue[i] = blueColor.CurrnetValue[i] - blueColor.ChangeSpeed[i];
		}
		else if(blueColor.CurrnetValue[i] < blueColor.DestinationValue[i])
		{
			blueColor.CurrnetValue[i] = blueColor.CurrnetValue[i] + blueColor.ChangeSpeed[i];
		}
		else
		{

		}
	}

}

void LedDummyHSVTest()
{
	int i,j;
	for(i = 0;i<360;i++)
	{
		HSV_to_RGB();

		for(j=0;j<64;j++)
			hsvHue.CurrnetValue[j]++;
	}

	for(j=0;j<64;j++)
		hsvHue.CurrnetValue[j] = 0;
}
