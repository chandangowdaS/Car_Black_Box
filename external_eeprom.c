/*
 * File:   external_eeprom.c
 * Author: subramanian t
 *
 * Created on 25 May, 2023, 10:46 AM
 */


#include <xc.h>

#include "main.h"


void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
    for (unsigned int wait = 3000;wait--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_rep_start();
    i2c_write(SLAVE_READ_E);
    data = i2c_read();
    i2c_stop();

    return data;
}
