#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"stm32f4xx_rng.h"
#include"stm32f4xx_tim.h"
#include"stm32f4xx_usart.h"
#include <assert.h>
#include  "misc.h"


#include"main.h"
#include "LedDriver.h"
#include "LedMode.h"

RedColor redColor = {};
GreenColor greenColor = {};
BlueColor blueColor = {};
HSVHue	hsvHue = {};
HSVSaturation hsvSaturation = {};
HSVValue hsvValue = {};
ProgramOptions programOptions = {};

volatile uint32_t time = 0;
volatile uint16_t pwmTimer = 0;

volatile bool refreshLed = TRUE;
uint16_t refreshProgram = 0;

int main(void)
{
	RGBTreeInitialization();
	RGBTreeWorkingMode();
}

void RGBTreeInitialization()
{
	SystemInit();
	SysTick_Init();
	init_MainTimer();
	init_GPIO();
	init_BT();

	RNG_Config();

	int i;
	for(i = 0;i<64;i++)
		redColor.ChangeSpeed[i] = 1;

	for(i = 0;i<64;i++)
		greenColor.ChangeSpeed[i] = 1;

	for(i = 0;i<64;i++)
		blueColor.ChangeSpeed[i] = 1;

	for(i = 0;i<64;i++)
		hsvSaturation.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvValue.CurrnetValue[i] = 100;

	for(i = 0;i<64;i++)
		hsvHue.CurrnetValue[i] = i;

	for(i = 0;i<64;i++)
		hsvHue.ChangeSpeed[i] = 1;

	programOptions.Mode = HSVtoRGBwithDelay;

	delay_ms(10);
}

void RGBTreeWorkingMode()
{
    while(1)
    {
    	switch(programOptions.Mode)
    	{
    		case HSVtoRGBwithDelay:
    			LedModeHSVtoRGBwithDelay();
    			break;
    		case HSVtoRGB:
    			LedModeHSVtoRGB();
    			break;
    		case FourColorTest:
    			LedDriverTest();
    			break;
    		case DummyRGBwithPWM:
    			LedDummyPWMTest();
    			break;
    		case FourSeasons:
    			LedFourSeason();
    			break;
    		case Strobo:
    			LedStrobo();
    		case SevenColorMode:
    			LedSevenColorMode();
    			break;
    		case Random:
    			LedRandom();
    			break;

    	}
    }
}

void init_GPIO()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitDef; //inicjalizacja struktury

	//Definicja wyjsc steruj¹cych kierunkiem obrotu silników
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;// ustawiam jako wyjscia porty PD5,6,7
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT; 	// wyjscie
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;	//typ push/pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;	//bez pullup
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_25MHz; //czêstotliowsc
	GPIO_Init(GPIOE, &GPIO_InitDef);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;// ustawiam jako wyjscia porty PD5,6,7
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT; 	// wyjscie
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;	//typ push/pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;	//bez pullup
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_25MHz; //czêstotliowsc
	GPIO_Init(GPIOD, &GPIO_InitDef);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;// ustawiam jako wyjscia porty PD5,6,7
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT; 	// wyjscie
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;	//typ push/pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;	//bez pullup
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_25MHz; //czêstotliowsc
	GPIO_Init(GPIOC, &GPIO_InitDef);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;// ustawiam jako wyjscia porty PD5,6,7
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT; 	// wyjscie
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;	//typ push/pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;	//bez pullup
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_25MHz; //czêstotliowsc
	GPIO_Init(GPIOA, &GPIO_InitDef);

}

void init_BT()
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;

	  //configure clock for USART
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	  //configure clock for GPIO
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	  USART_InitStructure.USART_BaudRate = 9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  //   GPIO_InitTypeDef GPIO_InitStructure;

	  //configure AF
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

	  //configure ports, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  USART_Init(USART3, &USART_InitStructure);

	  // Enable USART
	  USART_Cmd(USART3, ENABLE);
	  //USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	  //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


	  //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	  NVIC_InitTypeDef NVIC_InitStruct;

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	  NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	  NVIC_Init(&NVIC_InitStruct);

	  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	  USART_ITConfig(USART3, USART_IT_ERR, ENABLE);
}



/*
 * timer odpowiedzialny za sprawdzanie stanu czujników
 * byæ mo¿e w pszysz³oci bêdzie trzeba go zmieniæ na przerwanie
 */
void init_MainTimer()
{
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 84-1;	//f APB1 = 48MHz -> 1MHz
	timerInitStructure.TIM_Period = 38; //Need ~25600Hz
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(int ms)
{
	time = ms * 1000;
	while(time>0)
	{

	}

	return;
}

void delay_us(int us)
{
	time = us;
	while(time>0)
	{

	}

	return;
}

void SysTick_Init(void) {
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/
	while (SysTick_Config(SystemCoreClock / 1000000) != 0);// One SysTick interrupt
}


void RNG_Config(void)
{
 /* Enable RNG clock source */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

  /* RNG Peripheral enable */
  RNG_Cmd(ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		refreshLed = TRUE;
		pwmTimer++;

		if(pwmTimer > 256)
		{
			pwmTimer = 0;
		}
	}
}

void SysTick_Handler(void)
{
	time--;
}

void __attribute__((noreturn)) assert_failed (uint8_t* file, uint32_t line)
{
  /*trace_printf ("assert_param() failed: file \"%s\", line %d\n", file, line);*/
  abort ();
  /* NOTREACHED */
}

void __attribute__((noreturn)) __assert_func (const char *file, int line, const char *func,
               const char *failedexpr)
{
 /* trace_printf ("assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
                failedexpr, file, line, func ? ", function: " : "",
                func ? func : "");*/
  abort ();
  /* NOTREACHED */
}
