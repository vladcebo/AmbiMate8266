#ifndef AMBIMATE_H_
#define AMBIMATE_H_

#include "I2CSensor.h"

#define AMBIMATE_ADDR 0x2A

#define AMBIMATE_START_SCAN_REG 0xC0
#define AMBIMATE_SCANN_ALL      0xFF
#define AMBIMATE_VERS_REG       0x80
#define AMBIMATE_SUBVERS_REG    0x81 

#define AMBIMATE_DEFAULLT_VERS  28


/* Note that the minimum sample rate is 0.5 sec,
except for VOC/eCO2 where it is 60 sec */
class AmbiMate : public I2CSensor {

public:
    AmbiMate();

    bool init();

    void scanAll();
    void sampleAll();

    uint8_t getStatus();
    float   getTemperature();
    float   getHumidity();
    float   getLight();
    float   getAudio();
    float   getBattery();
    float   getECO2();
    float   getVOC();

private:
    uint8_t status;
    float temperature;
    float humidity;
    float light;
    float audio;
    float battery;
    float eco2;
    float voc;

};

#endif