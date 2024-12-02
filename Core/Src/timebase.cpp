#include "timebase.h"
#include "stm32f4xx.h"

#define MAX_DELAY               0xFFFFFFFFU

// One second is 16 million clock cycles
#define ONE_SECOND_LOAD         16000000

#define STCR_ENABLE             (1U<<0)
#define STCR_IRQ_EN             (1U<<1)
#define STCR_CLK_SOURCE         (1U<<2)
#define STCR_CF                 (1U<<16)

volatile uint32_t currentTick;
volatile uint32_t tickFrequency = 1;

uint32_t getTick(void)
{
    __disable_irq();
    uint32_t tick = currentTick;
    __enable_irq();
    return tick;
}

void delay(uint32_t delay)
{
    uint32_t ticktstart = getTick();
    uint32_t wait = delay;
    if(delay < MAX_DELAY)
    {
        wait += (uint32_t)(tickFrequency);
    }

    while((getTick() - ticktstart) < wait){}
}

void incrementTick()
{
    currentTick += tickFrequency;
}


void timebaseInit(void)
{
    //Load systick timer
    SysTick->LOAD = ONE_SECOND_LOAD - 1; //Since 0 is included

    //Clear Current value register
    SysTick->VAL = 0;

    //Set systick to use processor clock
    SysTick->CTRL = STCR_CLK_SOURCE;

    //Enable systick interrupt
    SysTick->CTRL |= STCR_IRQ_EN;

    //Enable systick register
    SysTick->CTRL |= STCR_ENABLE;


    // // Enable countflag to determine if systick wa ever counted to zero
    // SysTick->CTRL |= STCR_CF;

    // Enable global interrupts
    __enable_irq();
}

void SysTick_Handler(void)
{
    incrementTick();
}