#include <stdint.h>
#include "syscalls.h"

/* GPIO LED pin definition */
#define LED_PIN 25
/* GPIO Button pin definition */
#define BTN_PIN 15
/* Debounce threshold: number of consecutive equal samples required */
#define DEBOUNCE_COUNT 2500

int user_app_entry(void) {
    // Setup Phase (Unprivileged): The user program configures its resources using syscalls.

    // Configure LED_PIN as output and BTN_PIN as input using sys_gpio_dir syscall
    sys_gpio_dir(LED_PIN, 1);   // LED_PIN as output
    sys_gpio_dir(BTN_PIN, 0);   // BTN_PIN as input

    // Bi-stable output state (toggle mode)
    int led_state = 0;
    sys_gpio_set(LED_PIN, led_state);

    // Debounce state
    int last_sample = sys_gpio_get(BTN_PIN);
    int stable_state = last_sample;
    int stable_counter = 0;

    while (1) {
        int sample = sys_gpio_get(BTN_PIN);

        if (sample == last_sample) {
            if (stable_counter < DEBOUNCE_COUNT) {
                stable_counter++;
            } else if (sample != stable_state) {
                // New debounced state detected
                stable_state = sample;

                // Falling edge (1 -> 0): toggle LED once per valid press
                if (stable_state == 0) {
                    led_state ^= 1;
                    sys_gpio_set(LED_PIN, led_state);
                }
            }
        } else {
            // Signal changed: restart debounce counter
            stable_counter = 0;
        }

        last_sample = sample;
    }
    
    // Should never reach here
    return 0;
}

