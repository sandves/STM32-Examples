#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "led_control.h"

static void init_gpio();
static void switch_color();
static void next_color(int next_color);

static int led_state = 0;
static int color = COLOR_RED;
static int color_count = 0;

void led_control_init()
{
	init_gpio();
}

static void init_gpio()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpioStruct;

	//External LEDs
	gpioStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_2;
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpioStruct);

	//Internal LEDs
	gpioStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpioStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpioStruct);

	//Turn off the RGB LED by setting all pins high
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
}

void toggle_led()
{

	GPIO_WriteBit(GPIOA, GPIO_Pin_8, led_state ? Bit_RESET : Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_8, led_state ? Bit_RESET : Bit_SET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_9, led_state ? Bit_RESET : Bit_SET);

	switch_color();

	led_state = !led_state;

}

static void switch_color()
{
	/*switch(color)
	{
	case COLOR_RED:
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, led_state ? Bit_RESET : Bit_SET);
		color = COLOR_GREEN;
		break;
	case COLOR_GREEN:
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, led_state ? Bit_RESET : Bit_SET);
		color = COLOR_BLUE;
		break;
	case COLOR_BLUE:
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, led_state ? Bit_RESET : Bit_SET);
		color = COLOR_RED;
		break;
	}*/

	switch(color)
	{
	case COLOR_RED:
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, led_state ? Bit_SET : Bit_RESET);
		next_color(COLOR_GREEN);
		break;
	case COLOR_GREEN:
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, led_state ? Bit_SET : Bit_RESET);
		next_color(COLOR_BLUE);
		break;
	case COLOR_BLUE:
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, led_state ? Bit_SET : Bit_RESET);
		next_color(COLOR_RED);
		break;
	}
}

static void next_color(int next_color)
{
	if(color_count == 1)
	{
		color = next_color;
		color_count = 0;
	}
	else
	{
		color_count++;
	}
}
