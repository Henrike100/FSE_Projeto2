#ifndef UTILIDADES_HPP
#define UTILIDADES_HPP

#include <unistd.h>
#include "bme280.hpp"

struct identifier {
    uint8_t dev_addr;
    int8_t fd;
};

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

#endif // UTILIDADES_HPP
