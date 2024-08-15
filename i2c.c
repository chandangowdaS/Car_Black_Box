#include "main.h"
#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;  //to check whether data has been updated in registers
	/* Clear bus collision flag */
	BCLIF = 0;
}

void i2c_idle(void)
{
	while (!SSPIF);  //check bus condition by flag
	SSPIF = 0;
}

void i2c_ack(void)
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}

void i2c_start(void)
{
	SEN = 1;  //to start communication
	i2c_idle();
}

void i2c_stop(void)
{
	PEN = 1;  //to stop communication
	i2c_idle();
}

void i2c_rep_start(void)
{
	RSEN = 1;   //to stop communication /disable repeat stack
	i2c_idle();
}

void i2c_write(unsigned char data)
{
	SSPBUF = data;  //write data to buffer
	i2c_idle();
}

void i2c_rx_mode(void)
{
	RCEN = 1;  //receive enable for slave
	i2c_idle();
}

void i2c_no_ack(void)
{
	ACKDT = 1;  //don't use ack by mot acknowledging it
	ACKEN = 1;  //enable ack
}

unsigned char i2c_read(void)
{
	i2c_rx_mode();  //receive enable for slave
	i2c_no_ack();  //don't use ack

	return SSPBUF;  //return the content
}