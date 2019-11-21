#include "freertos/FreeRTOS.h"
#include "esp_common.h"
#include "gpio.h"

extern "C" {
    void user_init(void);
    uint32 user_rf_cal_sector_set(void);
}

#define LED_PIN GPIO_Pin_2

uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void test_task(void *pvParameters) {
    GPIO_AS_OUTPUT(LED_PIN);
    int led_status = 0;
    while (1) {
        led_status ^= 1;
        GPIO_OUTPUT(LED_PIN, led_status);
        os_printf("I'm ok. Thank you\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void user_init(void) {
    os_printf("SDK version:%s\n", system_get_sdk_version());
    xTaskCreate(test_task, (const signed char*)"My task", 100, NULL, 1, NULL);
}

