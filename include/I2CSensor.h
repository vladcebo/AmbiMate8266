#include <cstdint>
#include "I2C.h"

/* A generic I2C sensor that exports some methods to work with general-purpose
   I2C sensors */
class I2CSensor {

public:
    I2CSensor(uint8_t i2c_addr) {
        this->i2c_addr = i2c_addr;
    }

    /* Sensor dependent method used for initialization */
    virtual bool init() = 0;

    /* Write data to a sensor register reg_addr */
    void writeReg(uint8_t reg_addr, uint8_t data)
    {
        uint8_t buff[2] = {reg_addr, data}; 
        I2C::write(i2c_addr, buff, 2, true);
    }

    /* Read a register from the sensor */
    uint8_t readReg(uint8_t reg_addr)
    {
        uint8_t data;
        I2C::write(i2c_addr, &reg_addr, 1, false);
        I2C::read(i2c_addr,  &data, 1);

        return data;
    }

    /* Read multiple registers from the sensor into the buffer
       Note: it makes sense only in case the sensors support autoincrement of index on reading */
    void readMultiple(uint8_t addr, uint8_t buff[], uint8_t len)
    {
        I2C::write(i2c_addr, &addr, 1, false);
        I2C::read(i2c_addr,  buff, len);
    }


private:
    uint8_t i2c_addr; 

};