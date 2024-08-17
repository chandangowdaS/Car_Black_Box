/* 
 * File:   external_eeprom.h
 * Author: subramaniant
 *
 * Created on 25 May, 2023, 10:47 AM
 */
#ifndef external
#define external

#define SLAVE_READ_E		0xA1
#define SLAVE_WRITE_E		0xA0


void write_external_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);

#endif
