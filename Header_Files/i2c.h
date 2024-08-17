#ifndef I2C_H
#define I2C_H


void init_i2c(void);  //initialisation
void i2c_start(void);  //start communication
void i2c_rep_start(void);  //repeat satck
void i2c_stop(void);  //stop communication
void i2c_write(unsigned char data); //write to slave by putting data intobuffer
unsigned char i2c_read(void);   //read from slave from buffer

#endif