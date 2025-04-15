#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"
#include "esp_task_wdt.h"

// Timer Configuration Macros
#define TIMER_DIVIDER         80         // Timer clock divider: APB_CLK (80 MHz) / 80 = 1 MHz (1 tick = 1 µs)
#define TIMER_INTERVAL_SEC    1          // Timer interrupt interval: 1 second
#define TIMER_GROUP           TIMER_GROUP_0
#define TIMER_IDX             TIMER_0    // Using hardware timer 0 from timer group 0

// Global Shared Values
volatile uint32_t interrupt_counter = 0; // Counter incremented on every timer interrupt (must be volatile)
bool print = 0;                          // Flag set in ISR to signal main loop to print

// Timer ISR (Interrupt)
/**
 * @brief Timer interrupt service routine (ISR)
 * 
 * This ISR is triggered every TIMER_INTERVAL_SEC (1 second).
 * It increments a counter and sets a flag for the main loop to act on.
 */
static void IRAM_ATTR timer_isr_callback(void *args)
{
    interrupt_counter++; // Increment counter every second
    print = 1;           // Signal main task to print the counter
}

void app_main()
{
    // Configure Timer
    timer_config_t config;
    config.alarm_en = TIMER_ALARM_EN;            // Enable alarm (interrupt)
    config.counter_en = TIMER_PAUSE;             // Timer initially paused
    config.intr_type = TIMER_INTR_LEVEL;         // Level interrupt type
    config.counter_dir = TIMER_COUNT_UP;         // Count-up mode
    config.auto_reload = TIMER_AUTORELOAD_EN;    // Automatically reload counter on alarm
    config.divider = TIMER_DIVIDER;              // Clock divider (1 µs per tick)

    // Initialize hardware timer
    timer_init(TIMER_GROUP, TIMER_IDX, &config);

    // Reset timer counter value to 0
    timer_set_counter_value(TIMER_GROUP, TIMER_IDX, 0);

    // Set alarm to trigger after TIMER_INTERVAL_SEC seconds (in µs)
    timer_set_alarm_value(TIMER_GROUP, TIMER_IDX, TIMER_INTERVAL_SEC * 1000000);

    // Enable timer interrupt and register ISR callback
    timer_enable_intr(TIMER_GROUP, TIMER_IDX);
    timer_isr_callback_add(TIMER_GROUP, TIMER_IDX, timer_isr_callback, NULL, 0);

    // Start the timer
    timer_start(TIMER_GROUP, TIMER_IDX);

    // Configure Task Watchdog Timer (TWDT)
    /**
     * Watchdog monitors the main task.
     * If esp_task_wdt_reset() is not called within 3 seconds, the WDT will trigger a panic/reset.
     */
    esp_task_wdt_init(3, true);  // 3s timeout, panic on timeout enabled
    esp_task_wdt_add(NULL);      // Add current task (main) to the watchdog

    while (1) {
        // Print only when ISR signals to do so
        if (print) {
            print = 0;
            printf("Interrupt count: %d\n", interrupt_counter);
            vTaskDelay(pdMS_TO_TICKS(5000));

        }

        // Feed the watchdog to indicate task is alive
        esp_task_wdt_reset();
    }
}
