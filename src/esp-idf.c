#include <stdio.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <esp_system.h>
#include "esp_timer.h"


// Define compile-time switch for DEBUG timing
#define DEBUG_TIMING

// Timing parameters

#define TON1_DEBUG       100000  // DEBUG on-time in microseconds
#define TOFF_DEBUG       100000  // DEBUG off-time in microseconds
#define NUM_PULSES_DEBUG 10      // DEBUG number of pulses
#define IDLE_TIME_DEBUG  200000  // DEBUG idle time between DATA and SYNC pulses
#define TSYNC_ON_DEBUG   500000  // DEBUG SYNC on-time



// Define GPIO pin numbers
const gpio_num_t PB1_PIN = GPIO_NUM_2;
const gpio_num_t PB2_PIN = GPIO_NUM_3;
const gpio_num_t DATA_PIN = GPIO_NUM_9;
const gpio_num_t SYNC_PIN = GPIO_NUM_10;

// Operational state
enum SystemState {
    STATE_IDLE,
    STATE_OUTPUT_ENABLED,
    STATE_ALTERNATIVE_SELECTED
};


// Variable to store the User's Surname
char userSurname[] = "ab";

// Variable to store the current state
enum SystemState currentState = STATE_IDLE;

// Variable to store the alternative behavior
int alternativeBehavior;

// Function prototypes
void handlePushButton1();
void handlePushButton2();
void generateDataWaveform();
void generateAlternativeDataWaveform(int alternativeBehavior);
void generateSyncPulse();
void calculateTimingParameters();
int64_t esp_timer_get_time(void);

void app_main() {
    // Set up the variables 
    alternativeBehavior = 0;

    // Set pin modes
    gpio_set_direction(PB1_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(PB2_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SYNC_PIN, GPIO_MODE_OUTPUT);

    // Main loop
    while (1) {
        // Check and handle push button inputs
        vTaskDelay(1);  // Small delay for task yield
        handlePushButton1();
        handlePushButton2();
        ESP_LOGI("Main Loop", "Current State : %d", currentState);

        // Generate appropriate waveforms based on the current state
        switch (currentState) {
            case STATE_OUTPUT_ENABLED:
                generateDataWaveform();
                break;
            case STATE_ALTERNATIVE_SELECTED:
                generateAlternativeDataWaveform(alternativeBehavior);
                break;
            default:
                // Handle other states or do nothing in STATE_IDLE
                // Generate SYNC pulse
                generateSyncPulse();
                break;
        }

        //calculateTimingParameters(userSurname);
        //vTaskDelay(5000 / portTICK_PERIOD_MS);  // Delay for better readability
    }
}

void handlePushButton1() {
    int64_t lastDebounceTimeUp = 0;
    int64_t debounceDelay = 500;

    if (gpio_get_level(PB1_PIN) == 1 && currentState == STATE_IDLE) {
        lastDebounceTimeUp = esp_timer_get_time();
    }

    if ((esp_timer_get_time() - lastDebounceTimeUp) > debounceDelay) {
        if (gpio_get_level(PB1_PIN) == 0) {
            currentState = STATE_OUTPUT_ENABLED;
            ESP_LOGI("PushButton1", "Pressed");
        }
    }
}

void handlePushButton2() {
    int64_t lastDebounceTimeUp = 0;
    int64_t debounceDelay = 500;

    if (gpio_get_level(PB2_PIN) == 1 && currentState == STATE_IDLE) {
        lastDebounceTimeUp = esp_timer_get_time();
    }

    if ((esp_timer_get_time() - lastDebounceTimeUp) > debounceDelay) {
        if (gpio_get_level(PB2_PIN) == 0) {
            currentState = STATE_ALTERNATIVE_SELECTED;
            alternativeBehavior += 1;
            alternativeBehavior = (alternativeBehavior % 4);
            ESP_LOGI("PushButton2", "Pressed");
            ESP_LOGI("AlternativeBehaviour", "%d", alternativeBehavior);
        }
    }
}

void generateDataWaveform() {
    int i = 0;
    while (i < NUM_PULSES_DEBUG) {
        i += 1;
        gpio_set_level(DATA_PIN, 1);
        vTaskDelay(TON1_DEBUG);

        gpio_set_level(DATA_PIN, 0);
        vTaskDelay(TOFF_DEBUG);
        ESP_LOGI("DataWaveform", "Generated");
    }
    currentState = STATE_IDLE;
    vTaskDelay(IDLE_TIME_DEBUG);
}

void generateAlternativeDataWaveform(int alternativeBehavior) {
    switch (alternativeBehavior) {
        case 1:
            for (int i = 0; i < NUM_PULSES_DEBUG - 3; ++i) {
                gpio_set_level(DATA_PIN, 1);
                vTaskDelay(TON1_DEBUG);

                gpio_set_level(DATA_PIN, 0);
                vTaskDelay(TOFF_DEBUG);
            }
            break;

        case 2:
            for (int i = NUM_PULSES_DEBUG; i > 0; --i) {
                gpio_set_level(DATA_PIN, 1);
                vTaskDelay(TON1_DEBUG);

                gpio_set_level(DATA_PIN, 0);
                vTaskDelay(TOFF_DEBUG);
            }
            break;

        case 3:
            for (int i = 0; i < NUM_PULSES_DEBUG + 3; ++i) {
                gpio_set_level(DATA_PIN, 1);
                vTaskDelay(TON1_DEBUG);

                gpio_set_level(DATA_PIN, 0);
                vTaskDelay(TOFF_DEBUG);
            }
            break;

        default:
            generateDataWaveform();
            break;
    }
    currentState = STATE_IDLE;
    vTaskDelay(IDLE_TIME_DEBUG);
}

void generateSyncPulse() {
    int64_t TSYNC_ON = TSYNC_ON_DEBUG;
    gpio_set_level(SYNC_PIN, 1);
    vTaskDelay(TSYNC_ON);

    gpio_set_level(SYNC_PIN, 0);
}
