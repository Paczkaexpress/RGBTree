#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"stm32f4xx_rng.h"
#include"stm32f4xx_tim.h"
#include"stm32f4xx_usart.h"
#include <assert.h>
#include  "misc.h"

#include"main.h"
#include "Led.h"

extern ProgramOptions programOptions;

int znak;
char StringLoop[20];

void blueToothHander(char znak)
{
	if(znak == 10) // komenda zmiany wartosci hue
	{

	}
	else if(znak == 11)  // komenda zmiany wartosci saturation
	{

	}
	else if(znak == 12)  // komenda zmiany wartosci value
	{

	}
	else
		programOptions.Mode = znak; //ustwiam tryb pracy
}

void Usart_SendString(char* str)
{

	while(*str)
	{
		while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData(USART3, *str++);
	}
}

void Usart_SendValue(int val)
{
	int i=0;
	int tab[16];

	for(i=0;val>0;i++)
	{
		tab[i] = val%10;
		val = val/10;
	}

	for(;i>0;i--)
	{
		while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
		USART_SendData(USART3, tab[i-1] + 48);
	}
	while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
	USART_SendData(USART3, ' ');
}

void USART3_IRQHandler(void)
{
  static int tx_index = 0;
  static int rx_index = 0;

  /*if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET) // Transmit the string in a loop
  {
    USART_SendData(USART3, StringLoop[tx_index++]);

    if (tx_index >= (sizeof(StringLoop) - 1))
      tx_index = 0;
  }*/

  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // Received characters modify string
  {
    znak = USART_ReceiveData(USART3);
    blueToothHander(znak);
  }
}
