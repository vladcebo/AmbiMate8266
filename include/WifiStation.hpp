#ifndef WIFI_STATION_H_
#define WIFI_STATION_H_

#include "esp_common.h"

class WifiStation {

public:

    static void start(const char* ssid, const char* password) {
        connected = false;
        struct station_config config;

        bzero(&config.ssid,      sizeof(struct station_config));
        strncpy(config.ssid,     ssid, 32);
        strncpy(config.password, password, 64);
        wifi_station_set_config(&config);
        wifi_set_event_handler_cb(WifiStation::wifi_event_handler_cb);
    }

    static bool isConnected() {
        return connected;
    }

private:
    static bool connected;

    static void wifi_event_handler_cb(System_Event_t *event) {
        if (event == NULL) {
            return;
        }
        switch (event->event_id) {
            case EVENT_STAMODE_CONNECTED:
                os_printf("Connected to Wifi\n");
                break;
            case EVENT_STAMODE_DISCONNECTED:
                connected = false;
                break;
            case EVENT_STAMODE_GOT_IP:
                os_printf("Got IP\n");
                connected = true;
                break;
            default:
                break;
        }
    }

    WifiStation() {
    }
};

bool WifiStation::connected = false;

#endif