#include "uart.h"
#include "stm32f4xx.h"

#define GPIOAEN             (1U<<0)
#define UART2EN             (1U<<17);

#define SYS_FREQUENCY       16000000
#define APB1_CLK            SYS_FREQUENCY
#define UART_BAUDRATE       115200

#define USART_CR1_TE_EN     (1U<<3)
#define USART_CR1_UE_EN     (1U<<13)

#define USART_SR1_TXE       (1U<<7)

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

static void setUartBaudrate(uint32_t peripheralClock, uint32_t baudrate);
static uint32_t computeUartBaudrate(uint32_t &peripheralClock, uint32_t &baudrate);
static void uartWrite(int character);

// Remapping printf
extern "C"
{
    PUTCHAR_PROTOTYPE
    {
        uartWrite(ch);
        return ch;
    }
}

void uartTransmitInit(void)
{
    // Enbale clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // Set PA2 mode to AF mode
    GPIOA->MODER &= ~(1U<<4);
    GPIOA->MODER |= (1U<<5);

    // Set AF to AF07 (UART TX)
    GPIOA->AFR[0] |= (1U<<8);
    GPIOA->AFR[0] |= (1U<<9);
    GPIOA->AFR[0] |= (1U<<10);
    GPIOA->AFR[0] &= ~(1U<<11);

    //Enable clock access to UART 2
    RCC->APB1ENR |= UART2EN;

    // Configure baud rate
    setUartBaudrate(APB1_CLK, UART_BAUDRATE);

    // Set transfer direction to transmit (Setting all other bits to zero on purpose)
    USART2->CR1 = USART_CR1_TE_EN;

    // Enable uart module
    USART2->CR1 |= USART_CR1_UE_EN;
}

static void uartWrite(int character)
{
    // Make sure the transmit data Register is empty by looking at status register
    while(!(USART2->SR & USART_SR1_TXE));

    // write to transmit data register
    USART2->DR = (character & 0xFF);
}

static void setUartBaudrate(uint32_t peripheralClock, uint32_t baudrate)
{
    USART2->BRR = computeUartBaudrate(peripheralClock, baudrate);
}

static uint32_t computeUartBaudrate(uint32_t &peripheralClock, uint32_t &baudrate)
{
    return ((peripheralClock + (baudrate/2U))/baudrate);
}