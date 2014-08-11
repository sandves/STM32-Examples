#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "servo_control.h"
#include "led_control.h"
#include <stdbool.h>;

static void init_button_gpio();
static void init_timer();
static void enable_timer_interrupt();
static void delay(uint32_t time);

// __IO = volatile
// Use __IO to prevent the compiler from removing the loop
// in SystickHandle during code optimization.
static __IO uint32_t TimingDelay;
static bool running = true;
static bool button_pressed = false;
static uint32_t led_counter = 0;

int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);

	led_control_init();
	servo_control_init();
	init_button_gpio();
	init_timer();
	enable_timer_interrupt();

	while(1) {}
}

static void init_button_gpio()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioStruct;

	//External button
	gpioStruct.GPIO_Pin = GPIO_Pin_9;
	gpioStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpioStruct);
}

static void init_timer()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/*
	 * Setting up the timer to interrupt every 1000ms.
	 * Calculations:
	 * Timer frequency is 24 000 000 Hz
	 * We want it to interrupt one time per second -> 1 * 24 000 000
	 * Timer is 16 bits, so the highest value is 65535 (2^16).
	 * 24 000 000 = 24000 * 1000
	 * Prescaler = 24000 - 1 (zero based)
	 * Period = 1000 -1 (zero based)
	 */

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 1200-1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 1000-1;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);

	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

static void enable_timer_interrupt()
{
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void TIM2_IRQHandler()
{
	// The following code makes sure that the running state only changes on button release.
	// If we only poll the pin for a low state, we would accidentally change the running
	// state way to many times.
	// I am using a pull-up resistor for the button input, so the pin reads low when the
	// button is pressed.

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == Bit_RESET && !button_pressed)
	{
		button_pressed = true;
	}

	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == Bit_SET && button_pressed)
	{
		running = !running;
		button_pressed = false;
	}


	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (running == true)
		{
			// Move servo one step every timer interrupt.
			toggle_servo();

			// The following code makes the LEDs blink twice
			// followed by a longer pause.
			if(led_counter % 2 != 0 && led_counter <=7)
			{
				toggle_led();

			}
			if(led_counter == 20)
				led_counter = 0;
			led_counter++;
		}
	}
}

static void delay(uint32_t time)
{
	TimingDelay = time;
	while(TimingDelay != 0);
}

void SysTick_Handler()
{
	if (TimingDelay != 0x00)
		TimingDelay--;
}
