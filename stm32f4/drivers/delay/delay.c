#include "delay.h"
// cycles per microsecond
static uint32_t usTicks = 0;

// cycles per millisecond
static uint32_t msTicks = 0;

// current uptime for 1kHz systick timer. will rollover after 49 days. hopefully we won't care.
static volatile uint32_t sysTickUptime = 0;
void SysTick_Handler(void)
{
	sysTickUptime++;
}

void delay_init(void)
{
	// 1us时钟数
	usTicks = SystemCoreClock / 1000000;
	// 1ms时钟数
	msTicks = SystemCoreClock / 1000;
	// 1ms时基中断
	SysTick_Config(msTicks);
	//提高中断优先级，避免在其他中断中调用时计时出错
	NVIC_SetPriority(SysTick_IRQn, 0);
}

void delay_deinit(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//返回系统启动us数 (70分钟溢出)
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;

	do {
		ms = sysTickUptime;
		cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);

	return (ms * 1000) + (msTicks - cycle_cnt) / usTicks;
}

//返回系统启动毫秒数 (49天溢出)
uint32_t millis(void)
{
	return sysTickUptime;
}

// us级延时
void delay_us(uint32_t us)
{
	uint32_t now = micros();

	while (micros() - now < us);
}

// ms级延时
void delay_ms(uint32_t ms)
{
	while (ms--) {
		delay_us(1000);
	}
}

void delay_s(uint32_t s)
{
	while (s--) {
		delay_ms(1000);
	}
}
