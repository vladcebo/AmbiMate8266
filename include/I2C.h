#ifndef I2C_H_
#define I2C_H_

#include <cstdint>
#include "esp_common.h"
#include "gpio.h"

#define I2C_ADDR_WRITE_BIT 0
#define I2C_ADDR_READ_BIT  1

extern  "C" {
    #include "i2c_master.h"
}


/* Basic class wrapper around ESP8266 RTOS i2c_master library (one of the reasons why its made static)
   to provide a more abstract way to communicate with I2C port.
   No external pull up resistors are required, since internal ones are enabled.
   Note: this class is not parametrized and default pins are:
            GPIO_PIN_2 - SCL
            GPIO_PIN_4 - SDA
    They can be changed by apropriate redefines, but that requires to change
    the library header files, which is not a good practice. */
class I2C {

public:
    static void init() {
        i2c_master_gpio_init();
        PIN_PULLUP_EN(GPIO_PIN_REG_2);
        PIN_PULLUP_EN(GPIO_PIN_REG_4);
    }

    /* Read n bytes into dest[] buffer from the device given by the address addr */
    static void read(uint8_t addr, uint8_t dest[], uint8_t n) {
        i2c_master_start();
        i2c_master_writeByte(addr << 1 | I2C_ADDR_READ_BIT);
        i2c_master_checkAck();
        for (uint8_t i = 0; i < n - 1; i++) {
            dest[i] = i2c_master_readByte();
            i2c_master_send_ack();
        }
        dest[n - 1] = i2c_master_readByte();
        i2c_master_send_nack();
        i2c_master_stop();
    }

    /* Write n bytes from the data[] buffer to the device given by the address addr,
       stop_cond flag is used in case we wish to make a repeated start or to address
       another device without releasing the bus */
    static void write(uint8_t addr, uint8_t data[], uint8_t n, bool stop_cond) {
        i2c_master_start();
        i2c_master_writeByte(addr << 1 | I2C_ADDR_WRITE_BIT);
        i2c_master_checkAck();

        for (uint8_t i = 0; i < n; i++) {
            i2c_master_writeByte(data[i]);
            i2c_master_checkAck();
        }
        if (stop_cond)
            i2c_master_stop();
    }

private:
    I2C() {
    }

};

#endif 