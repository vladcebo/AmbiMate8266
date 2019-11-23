#include "freertos/FreeRTOS.h"
#include "esp_common.h"
#include "gpio.h"
#include "AmbiMate.h"
#include "wifi_config.h"
#include "espconn.h"
#include "WifiStation.hpp"

extern "C" {
    #include "tinystdio.h"
}

extern "C" {
    void   user_init(void);
}

static AmbiMate sens;

char buff[100];

void main_task(void *pvParameters) {

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
        tfp_sprintf(buff, "light       = %2.2f lux\n", sens.getLight());
        os_printf("%s", buff);

        if (WifiStation::isConnected()) {

        }
        /* Prepare and send data as a JSON */
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void user_init(void) {

    os_printf("SDK version:%s\n", system_get_sdk_version());

    espconn_init();
    I2C::init();
    WifiStation::start(WIFI_SSID, WIFI_PASSWORD);

    xTaskCreate(main_task, (const signed char*)"Main task", 500, NULL, 1, NULL);
}

