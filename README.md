# Ambibate8266

A small and simple demo project for reading data from an [AmbiMate](https://www.te.com/usa-en/products/sensors/multi-sensor-modules.html "AmbiMate") sensor on a ESP8266 (d1 mini) board. 

The project is written for [PlatformIO](https://platformio.org/ "PlatformIO") so it's easy just to import it into VisualCode and run it.

Http client is greatly inspired from [esphttpclient](https://github.com/Caerbannog/esphttpclient "esphttpclient"), some code is reused, but it's greatly reduced, supporting only post requests and basic error handling.

Since ESP8266 RTOS SDK doesn't support floating point formatting, the [tinystdio](https://github.com/vladcebo/TinyStdio "tinystdio") library is used.

The measurements are sent to [ThingsBoard](demo.thingsboard.io "ThingsBoard"), which is an online IoT platform. 

**Note**: for successfull compilation you need to create a file called "wifi_config.h" with your wifi credentials:
    #ifndef WIFI_CONFIG_H_
    #define WIFI_CONFIG_H
    
    #define WIFI_SSID      "your_wifi_name"
    #define WIFI_PASSWORD  "your_wifi_password"
    
    #endif

`

