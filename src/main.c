#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"

// MARK: Variable Definitions
volatile uint32_t millis;

// MARK: ISR Prototypes
void HardFault_Handler(void);
void SysTick_Handler(void);

int main(void)
{
    /* Enable SysTick for an interupt every millisecond */
    SysTick_Config(64000);

    BSP_LED_0_PORT->DIRSET = BSP_LED_0_MASK;

    uint32_t last_blink = 0;

    /* Main Loop */
    for (;;) {
        if ((millis - last_blink) > 1000) {
            last_blink = millis;
            BSP_LED_0_PORT->OUT ^= BSP_LED_0_MASK;
        }
        __WFI();
    }

	return 0;
}

/* Interupt Service Routines */
void SysTick_Handler(void)
{
    millis++;
}

void HardFault_Handler(void)
{
    // Flash the LED in a noticable pattern
    for (;;) {
        ;
    }
}
