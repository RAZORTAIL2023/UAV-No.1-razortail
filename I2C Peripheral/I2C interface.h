#ifndef I2C_INTERFACE_
#define I2C_INTERFACE_

#include "INIT.h"

void I2C_config(void);
void I2C_IF_write(uint8_t ADDRESS, uint8_t _register, uint8_t data);
uint8_t I2C_IF_read(uint8_t ADDRESS, uint8_t _register);

#endif
