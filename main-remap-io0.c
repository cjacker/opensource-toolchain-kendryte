// Demo to blink one LEDs connect to IO 17 on Yahboom K210 Developer Kit
// Here I avoid to blink the LED connect IO 0, since IO 0 of K210 is JTAG TCK.
//
// If Here blink the LED connect to IO 0, it will require a reset to enter JTAG debugging,
// And since to blink the LED, it will re-map IO 0, JTAG debugging is impossible.

#include <stdio.h>
#include <unistd.h>

#include "gpio.h"
#include "fpioa.h"

#define PIN_LED_0    (0)
#define PIN_LED_1   (17)

#define LED0_GPIONUM (0)
#define LED1_GPIONUM (1)

#define FUNC_LED0 (FUNC_GPIO0 + LED0_GPIONUM)
#define FUNC_LED1 (FUNC_GPIO0 + LED1_GPIONUM)


void hardware_init(void) 
{
    fpioa_set_function(PIN_LED_0, FUNC_LED0);
    fpioa_set_function(PIN_LED_1, FUNC_LED1);
}

int main(void) 
{
    hardware_init();

    gpio_init();
    gpio_set_drive_mode(LED0_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_drive_mode(LED1_GPIONUM, GPIO_DM_OUTPUT);

    gpio_pin_value_t value = GPIO_PV_HIGH;
    gpio_set_pin(LED0_GPIONUM, value);
    gpio_set_pin(LED1_GPIONUM, value);

    while (1) 
    {
        sleep(1);
        gpio_set_pin(LED0_GPIONUM, value);
        gpio_set_pin(LED1_GPIONUM, value = !value);
    }
    return 0;
}
