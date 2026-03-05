#include <stdint.h>
#include "syscalls.h"

/* GPIO LED pin definition */
#define LED_PIN 25
/* GPIO Button pin definition */
#define BTN_PIN 15

int user_app_entry(void) {
    // Setup Phase (Unprivileged): The user program configures its resources using syscalls.

    // Configure LED_PIN as output and BTN_PIN as input using sys_gpio_dir syscall
    sys_gpio_dir(LED_PIN, 1);   // LED_PIN as output
    sys_gpio_dir(BTN_PIN, 0);   // BTN_PIN as input

    while (1) {
        // Operational Phase (Unprivileged): The user program performs its main functionality.
        // Read the button state using sys_gpio_get 
        // set the LED state accordingly using sys_gpio_set
        int button_state = sys_gpio_get(BTN_PIN);
        sys_gpio_set(LED_PIN, button_state);
    }
    
    // Should never reach here
    return 0;
}

case SYS_GPIO_GET:
    // TODO: Read the GPIO value using the kernel function
    // Place the result back in r0 (svc_args[0]) to return to user space
    svc_args[0] = k_gpio_get(svc_args[0]);
    break;