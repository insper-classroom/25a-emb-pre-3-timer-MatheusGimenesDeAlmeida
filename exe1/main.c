#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;


volatile int flag_botao = 0;
volatile int flag_timer = 0;

repeating_timer_t timer;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        flag_botao = 1;
    } else if (events == 0x8) { // rise edge
    }
}

bool timer_callback(repeating_timer_t *rt) {
    flag_timer = 1;
    return true; 
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);  

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, btn_callback);

    while (true) {

        if (flag_botao) {
            flag_botao = 0;
            gpio_put(LED_PIN_R, 1);
            add_repeating_timer_ms(500, timer_callback, NULL, &timer);
        }

        if (flag_timer) {
            flag_timer = 0;
            cancel_repeating_timer(&timer);

            gpio_put(LED_PIN_R, 0);
        }
    }
}
