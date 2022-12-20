#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

void start();

void stop();

void write_byte(uint8_t byte);

uint8_t read_byte_with_ack();

uint8_t read_byte_without_ack();

#endif I2C_H