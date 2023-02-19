#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
const uint LED_PIN = 22;

//Pins for input
const uint BIT_1 = 2;
const uint BIT_2 = 3;
const uint BIT_3 = 4;
const uint SUPPLY = 5;

//Pins for output
const uint LED_0 = 6;
const uint LED_1 = 7;
const uint LED_2 = 8;
const uint LED_3 = 9;
const uint LED_4 = 10;
const uint LED_5 = 11;
const uint LED_6 = 12;
const uint LED_7 = 13;

void new_stripe(const int* led_array);
int calculate_speed();

const int LED_NUM = 8;
//Default speed
const int HOLD_TIME = 400000;

const int led_array[8] = {6, 7, 8, 9, 10, 11, 12, 13};
const int led_rev_array[8] = {13, 12, 11, 10, 9, 8, 7, 6};


int main() {
    stdio_init_all();

    gpio_init(BIT_1);
    gpio_set_dir(BIT_1, GPIO_IN);
    gpio_pull_down(BIT_1);
    gpio_init(BIT_2);
    gpio_set_dir(BIT_2, GPIO_IN);
    gpio_pull_down(BIT_2);
    gpio_init(BIT_3);
    gpio_set_dir(BIT_3, GPIO_IN);
    gpio_pull_down(BIT_3);
    gpio_init(SUPPLY);
    gpio_set_dir(SUPPLY, GPIO_OUT);
    gpio_put(SUPPLY, 1);

    gpio_init(LED_0);
    gpio_set_dir(LED_0, GPIO_OUT);
    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);
    gpio_init(LED_3);
    gpio_set_dir(LED_3, GPIO_OUT);
    gpio_init(LED_4);
    gpio_set_dir(LED_4, GPIO_OUT);
    gpio_init(LED_5);
    gpio_set_dir(LED_5, GPIO_OUT);
    gpio_init(LED_6);
    gpio_set_dir(LED_6, GPIO_OUT);
    gpio_init(LED_7);
    gpio_set_dir(LED_7, GPIO_OUT);

    while (1) {
        new_stripe(led_array);
        new_stripe(led_rev_array);
    }
}


/**
 * @brief Function creates a sequence (snake) of lights.
 * Sequence consists of three leds, first one with full brightness, second one with half
 * and the third one with quarter brightness.
*/
void new_stripe(const int* array){
    for (int i = 0; i < LED_NUM; ++i){
        absolute_time_t start_time = get_absolute_time();
        while(absolute_time_diff_us(start_time, get_absolute_time()) < HOLD_TIME / calculate_speed()){

            gpio_put(array[i], 1);
            if (i > 0){
                gpio_put(array[i - 1], 1);
            }
            if (i > 1){
                gpio_put(array[i - 2], 1);
            }

            sleep_ms(5);
            if (i > 1){
                gpio_put(array[i - 2], 0);
            }

            sleep_ms(5);
            if (i > 0){
                gpio_put(array[i - 1], 0);
            }

            sleep_ms(10);
            gpio_put(array[i], 0);
        }
    }
}

/**
 * @brief Reads input bits. Input number consists of 3 bits, with
 * BIT_3 being the most significant and BIT_1 the least significant.
 * The result number serves as an exponent for divisor (base = 2).
 * In new_stripe() function, the divisor divides HOLD_TIME, which increases 
 * the speed of "snake".
*/
int calculate_speed(){
    int divisor = 1;
    int exponent = gpio_get(BIT_3) ? 1 : 0;
    exponent <<= 1;
    exponent |= gpio_get(BIT_2) ? 1 : 0;
    exponent <<= 1;
    exponent |= gpio_get(BIT_1) ? 1 : 0;
    divisor <<= exponent;
    return divisor;
}
