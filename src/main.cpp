#include "freertos/FreeRTOS.h"
#include "esp_common.h"
#include "gpio.h"
#include "AmbiMate.h"
#include "wifi_config.h"
#include "WifiStation.hpp"
#include "HttpClient.hpp"

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

            /* Prepare payload */
            tfp_sprintf(buff, "{\"temperature\": %2.2f, \"humidity\" : %2.2f, \"light\": %2.2f}",
                sens.getTemperature(), sens.getHumidity(), sens.getLight());
            HttpClient::post("demo.thingsboard.io", 80, "/api/v1/giTqgcEj2cDdoYj8XcO4/telemetry", 
            "Content-Type:application/json\n", buff);
        }
        /* Prepare and send data as a JSON */
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}


void user_init(void) {

    os_printf("SDK version:%s\n", system_get_sdk_version());

    I2C::init();
    WifiStation::start(WIFI_SSID, WIFI_PASSWORD);
    HttpClient::init();

    xTaskCreate(main_task, (const signed char*)"Main task", 2000, NULL, 1, NULL);
}

