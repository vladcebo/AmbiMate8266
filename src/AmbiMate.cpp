#include "AmbiMate.h"


AmbiMate::AmbiMate(): I2CSensor(AMBIMATE_ADDR) {
}

 // or we can return an error code that the version is wrong
bool AmbiMate::init() {
    // Firwmare version could be checked for example
    uint8_t version = readReg(AMBIMATE_VERS_REG)*10 + readReg(AMBIMATE_SUBVERS_REG);
    os_printf("%d\n", version);
    if (version != AMBIMATE_DEFAULLT_VERS) {
        return false;  
    }
    return true;
}

void AmbiMate::scanAll() {
    writeReg(AMBIMATE_START_SCAN_REG, AMBIMATE_SCANN_ALL);
}

void AmbiMate::sampleAll() {

    uint8_t buff[15];
    readMultiple(0x00, buff, 15);

    status      = buff[0];
    temperature = (buff[1] << 8 | buff[2])/10.0;
    humidity    = (buff[3] << 8 | buff[4])/10.0;
    light       = (float) (buff[5] << 8 | buff[6]);
    audio       = (float) (buff[7] << 8 | buff[8]);
    battery     = ((buff[9] << 8 | buff[10])/1024.0) * (3.3 / 0.330);
    eco2        = (float) (buff[11] << 8 | buff[12]);
    voc         = (float) (buff[13] << 8 | buff[14]);
}

uint8_t AmbiMate::getStatus() {
    return status;
}

float AmbiMate::getTemperature() {
    return temperature;
}

float AmbiMate::getHumidity() {
    return humidity;
}

float AmbiMate::getLight() {
    return light;
}

float AmbiMate::getAudio() {
    return audio;
}

float AmbiMate::getBattery() {
    return battery;
}

float AmbiMate::getECO2() {
    return eco2;
}

float AmbiMate::getVOC() {
    return voc;
}
