#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h."
#include <assert.h>

#include"stm32f4xx_tim.h"
#include"main.h"
#include"LedDriver.h"
#include"LedMode.h"

#define REFRESH_EVERY_MS(MS_SECOND) if(refreshProgram % MS_SECOND == 1)
#define MS_SECOND (a * 1000 / 25600)
#define MODE_HAS_CHANGED() 		if(actualMode != programOptions.Mode) break


extern RedColor redColor;
extern GreenColor greenColor;
extern BlueColor blueColor;
extern HSVHue	hsvHue;
extern HSVSaturation hsvSaturation;
extern HSVValue hsvValue;
extern ProgramOptions programOptions;

extern volatile bool refreshLed;
extern uint16_t refreshProgram;

int actualMode = 0;

/*
 *  1 program
 */

void LedModeHSVtoRGBwithDelay()
{
	int i,j;
	actualMode = programOptions.Mode;

	for(i = 0;i<64;i++)
		hsvSaturation.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvValue.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvHue.CurrnetValue[i] = 2*i;

	while(1)
	{
	if(refreshLed == TRUE)
	{
		refreshLed = FALSE;
		LED1_TOGGLE();

		REFRESH_EVERY_MS(100)
		{
				HSV_to_RGB();

				for(j=0;j<64;j++)
					hsvHue.CurrnetValue[j]++;

    			for(j=0;j<64;j++)
    				if(hsvHue.CurrnetValue[j]>360)
    					hsvHue.CurrnetValue[j] = 0;
		}
		if(refreshProgram % 50 == 1)
		{
			//UpdateLedValues();
		}
		if(refreshProgram > 25600)
		{
			refreshProgram = 0;
		}

		redLedDriver();
		greenLedDriver();
		blueLedDriver();

		refreshProgram++;
	}

	MODE_HAS_CHANGED();

	}
}



/*
 *  2 program
 */

void LedModeHSVtoRGB()
{
	int i,j;
	actualMode = programOptions.Mode;

	for(i = 0;i<64;i++)
		hsvSaturation.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvValue.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvHue.CurrnetValue[i] = 0;

	while(1)
	{
		if(refreshLed == TRUE)
		{
			refreshLed = FALSE;
			LED1_TOGGLE();

			if(refreshProgram % 200 == 1)
			{
				HSV_to_RGB();

				for(j=0;j<64;j++)
					hsvHue.CurrnetValue[j]++;

    			for(j=0;j<64;j++)
    				if(hsvHue.CurrnetValue[j]>360)
    					hsvHue.CurrnetValue[j] = 0;
			}
			if(refreshProgram > 25600)
			{
				refreshProgram = 0;
			}

			redLedDriver();
			greenLedDriver();
			blueLedDriver();

			refreshProgram++;
		}

		if(actualMode != programOptions.Mode)
		{
			break;
		}
	}
}

/*
 * 3 program
 */

void LedDriverTest()
{
	actualMode = programOptions.Mode;
	int i = 0;
	int color = RED;

	while(1)
	{
		if((color == RED) && (refreshProgram / 25600))
		{
			for(i = 0;i<64;i++)
				blueColor.CurrnetValue[i] = 0;

			for(i = 0;i<64;i++)
				redColor.CurrnetValue[i] = 100;

			color = GREEN;
		}
		else if((color == GREEN) && (refreshProgram / 25600))
		{
			for(i = 0;i<64;i++)
				redColor.CurrnetValue[i] = 0;

			for(i = 0;i<64;i++)
				greenColor.CurrnetValue[i] = 100;

			color = BLUE;
		}
		else if((color == BLUE) && (refreshProgram / 25600))
		{
			for(i = 0;i<64;i++)
				greenColor.CurrnetValue[i] = 0;

			for(i = 0;i<64;i++)
				blueColor.CurrnetValue[i] = 100;

			color = RED;
		}

		if(refreshProgram > 25600)
		{
			refreshProgram = 0;
		}

		redLedDriver();
		greenLedDriver();
		blueLedDriver();

		refreshProgram++;

		MODE_HAS_CHANGED();
	}
}

void LedDummyPWMTest()
{
	int i = 0;
	int j = 0;
	actualMode = programOptions.Mode;

	while(1)
	{
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

		if(actualMode != programOptions.Mode)
		{
			break;
		}
	}
}

void LedFourSeason()
{
	uint8_t Season = Spring;
	uint8_t i = 0;
	actualMode = programOptions.Mode;
	uint8_t ledChanged = 0;

	for(i = 0;i<64;i++)
		hsvSaturation.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvValue.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvHue.CurrnetValue[i] = 0;

	 while(1)
	 {

		 switch(Season)
		 {
		 case Spring:
				for(i = 0;i<64;i++)
				{
					while (RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET)
					{
						RNG_ClearFlag(RNG_FLAG_SECS);
						RNG_ClearFlag(RNG_FLAG_CECS);
						RNG_DeInit();
						RNG_Cmd(ENABLE);
					}
					hsvHue.DestinationValue[i] =  RNG_GetRandomNumber() % 20 + 80;
				}

			 while(1)
			 {
				 REFRESH_EVERY_MS(50)
				 {
					 UpdateHSVValues();
					 HSV_to_RGB();
				 }


				 if(refreshProgram > 25600)
				 {
					 refreshProgram = 0;
					 Season = Summer;
					 break;
				 }

					redLedDriver();
					greenLedDriver();
					blueLedDriver();

					refreshProgram++;

				 MODE_HAS_CHANGED();
			 }
			 break;

		 case Summer:

				for(i = 0;i<64;i++)
					hsvHue.DestinationValue[i] = hsvHue.DestinationValue[i] + 20;

			 while(1)
			 {
				 REFRESH_EVERY_MS(500)
				 {
					 UpdateHSVValues();
					 HSV_to_RGB();
				 }

				 if(refreshProgram > 25600)
				 {
					 refreshProgram = 0;
					 Season = Autum;
					 break;
				 }

					redLedDriver();
					greenLedDriver();
					blueLedDriver();

					refreshProgram++;

				 MODE_HAS_CHANGED();
			 }
			 break;

		case Autum:
			while (1) {
				REFRESH_EVERY_MS(100) {
					UpdateHSVValues();
					HSV_to_RGB();
				}
				REFRESH_EVERY_MS(300)
				{

					i = RandomNumber();
					while(hsvHue.DestinationValue[i] > 100)
					{

					}

					if(hsvHue.DestinationValue[i] > 100)
					{
						hsvHue.DestinationValue[i] = hsvHue.DestinationValue[i]
								- (RandomNumber() % 50) - 70;

						if(hsvHue.DestinationValue[i] > 100) hsvHue.DestinationValue[i] = 0;
					}

				}

				if (refreshProgram > 51200) {
					refreshProgram = 0;
					Season = Winter;
					break;
				}

				redLedDriver();
				greenLedDriver();
				blueLedDriver();

				refreshProgram++;

				MODE_HAS_CHANGED();
			}
			break;

		 case Winter:

				for(i = 0;i<64;i++)
					hsvHue.DestinationValue[i] = hsvHue.DestinationValue[i] + 150;

			 while(1)
			 {
				REFRESH_EVERY_MS(10) {
					UpdateHSVValues();
					HSV_to_RGB();
				}
				if (refreshProgram > 51200) {
					refreshProgram = 0;
					Season = Spring;
					break;
				}

				redLedDriver();
				greenLedDriver();
				blueLedDriver();

				refreshProgram++;

				MODE_HAS_CHANGED();
			}
			break;

		 }

		 MODE_HAS_CHANGED();
	 }
}

void LedSevenColorMode()
{
	int i;
	actualMode = programOptions.Mode;

	uint64_t redValue = 0;
	uint64_t greenValue = 0;
	uint64_t blueValue = 0;

	while(1)
	{

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
		if(actualMode != programOptions.Mode)
		{
			break;
		}
	}

}

void LedStrobo()
{
	actualMode = programOptions.Mode;

	while(1)
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

		if(actualMode != programOptions.Mode)
		{
			break;
		}
	}
}

void LedRandom()
{
	int i;
	actualMode = programOptions.Mode;

	uint64_t redValue = 0;
	uint64_t greenValue = 0;
	uint64_t blueValue = 0;

	while(1)
	{
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

		if(actualMode != programOptions.Mode)
		{
			break;
		}
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
	actualMode = programOptions.Mode;

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
}
