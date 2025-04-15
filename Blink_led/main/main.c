#include <stdio.h> // Standard input/output functions (not mandatory here but often included)

// FreeRTOS headers for delay and task management
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Header for GPIO control functions
#include "driver/gpio.h"

// Logging utility from ESP-IDF for debug messages
#include "esp_log.h"

// Define the GPIO pin number for the LED
// GPIO2 is commonly connected to the onboard LED in many ESP32 dev boards
#define BLINK_LED GPIO_NUM_2

// Function to initialize the LED GPIO pin as output
void gpioInt(void){

    // gpio_config_t is a structure that holds GPIO configuration
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,         // No interrupt is needed for blinking
        .pin_bit_mask = (1ULL << BLINK_LED),    // Create a bitmask for the LED pin
        .mode = GPIO_MODE_OUTPUT,               // Set pin mode as OUTPUT
        .pull_up_en = GPIO_PULLUP_DISABLE,      // Disable internal pull-up resistor
        .pull_down_en = GPIO_PULLDOWN_DISABLE   // Disable internal pull-down resistor
    };

    // Apply the above configuration to the GPIO
    gpio_config(&io_conf);
}

// Main function that runs after boot (like main() in normal C programs)
void app_main()
{
    // Initialize the LED pin
    gpioInt();

    // Variable to keep track of LED state (0 = OFF, 1 = ON)
    uint8_t led_state = 0;

    // Infinite loop to keep toggling the LED
    while(1){
        // Toggle the LED state using XOR (^= 1)
        // This flips the bit: 0 becomes 1, and 1 becomes 0
        gpio_set_level((gpio_num_t)BLINK_LED, led_state ^= 1); 
        // Log the LED state to the console
        ESP_LOGI("GPIO", "LED %s", led_state ? "ON" : "OFF");
        // Wait for 1000 milliseconds (1 second) before toggling again
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
