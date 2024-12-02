#include "led.h"

#define GPIOCEN (1U<<2)


void LEDInit(void)
{
    // Enable clock access to Port C by enabling it in the AHB1 enable register
    RCC->AHB1ENR |= GPIOCEN;
    // Set LED PIN 13 as output PIN by setting GPIOC mode register.27 to 0, 26 to 1
    GPIOC->MODER |= (1U<<26);
    GPIOC->MODER &= ~(1U<<27);
}

void LEDOn(void)
{
    // Set LED High (PIN 13)
    GPIOC->ODR |= (1U<<13);
}

void LEDOff(void)
{
    GPIOC->ODR &= ~(1U<<13);
}