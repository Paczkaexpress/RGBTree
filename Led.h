
/******************************************************
 * General definitions
 ******************************************************/

typedef uint8_t c_color;

/******************************************************
 * Pins names definitions
 ******************************************************/

#define LED1_SET() GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define LED2_SET() GPIO_SetBits(GPIOE, GPIO_Pin_5)

#define LED1_CLR() GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define LED2_CLR() GPIO_ResetBits(GPIOE, GPIO_Pin_5)

#define LED1_TOGGLE() GPIO_ToggleBits(GPIOE, GPIO_Pin_4)
#define LED2_TOGGLE() GPIO_ToggleBits(GPIOE, GPIO_Pin_5)

#define LED_OE_SET GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define LED_LA_SET GPIO_SetBits(GPIOA, GPIO_Pin_0)

#define LED_OE_CLR GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define LED_LA_CLR GPIO_ResetBits(GPIOA, GPIO_Pin_0)

#define LED_RED_OE_SET() 		GPIO_SetBits(GPIOD, GPIO_Pin_12)
#define	LED_RED_LA_SET()		GPIO_SetBits(GPIOD, GPIO_Pin_13)
#define	LED_RED_DATA_SET()		GPIO_SetBits(GPIOD, GPIO_Pin_15)
#define	LED_RED_CLK_SET()		GPIO_SetBits(GPIOD, GPIO_Pin_14)

#define LED_GREEN_OE_SET()		GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define	LED_GREEN_LA_SET()		GPIO_SetBits(GPIOC, GPIO_Pin_7)
#define	LED_GREEN_DATA_SET()	GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define	LED_GREEN_CLK_SET()		GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define LED_BLUE_OE_SET()		GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define	LED_BLUE_LA_SET()		GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define	LED_BLUE_DATA_SET()		GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define	LED_BLUE_CLK_SET()		GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define LED_RED_OE_CLR() 		GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define	LED_RED_LA_CLR()		GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define	LED_RED_DATA_CLR()		GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define	LED_RED_CLK_CLR()		GPIO_ResetBits(GPIOD, GPIO_Pin_14)

#define LED_GREEN_OE_CLR()		GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define	LED_GREEN_LA_CLR()		GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define	LED_GREEN_DATA_CLR()	GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define	LED_GREEN_CLK_CLR()		GPIO_ResetBits(GPIOC, GPIO_Pin_8)

#define LED_BLUE_OE_CLR()		GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define	LED_BLUE_LA_CLR()		GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define	LED_BLUE_DATA_CLR()		GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define	LED_BLUE_CLK_CLR()		GPIO_ResetBits(GPIOA, GPIO_Pin_11)

/******************************************************
 * Functions prototypes
 ******************************************************/

/******************************************************
 * Structure prototypes
 ******************************************************/
