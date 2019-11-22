#include "freertos/FreeRTOS.h"
#include "esp_common.h"
#include "gpio.h"
#include "AmbiMate.h"

extern "C" {
    #include "tinystdio.h"
}

extern "C" {
    void user_init(void);
    uint32 user_rf_cal_sector_set(void);
}

static AmbiMate sens;

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

char buff[100];

void test_task(void *pvParameters) {

    if (!sens.init()) {
        os_printf("Could not initialize AmbiMate sensor. Wrong version\n");
    }
    while (1) {
        sens.scanAll();
        vTaskDelay(500 / portTICK_RATE_MS);
        sens.sampleAll();
        tfp_sprintf(buff, "temperature = %2.2f C\n",   sens.getTemperature());
        os_printf("%s", buff);
        tfp_sprintf(buff, "humidity    = %2.2f %%\n",  sens.getHumidity());
        os_printf("%s", buff);
        tfp_sprintf(buff, "temperature = %2.2f lux\n", sens.getLight());
        os_printf("%s", buff);

        vTaskDelay(200 / portTICK_RATE_MS);
    }
}


void user_init(void) {
    os_printf("SDK version:%s\n", system_get_sdk_version());

    I2C::init();

    xTaskCreate(test_task, (const signed char*)"My task", 200, NULL, 1, NULL);
}

