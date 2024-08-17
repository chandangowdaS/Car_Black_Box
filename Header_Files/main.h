/* 
 * File:   main.h
 * Author: Chandan
 *
 * Created on 25 July, 2024, 12:21 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define MAX_SPEED 10.33
#define UP 0x80
#define DOWN 0xC0


#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "eeprom.h"
#include "uart.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "ds1307.h"


void init_timer0(void);

void read_pass_eeprom(void);

void write_log_eeprom(void);

void read_log_eeprom(void);

void get_time(void);

void display_time(void);

void clear_reset(void);

/*void view_log();
void download_log();
void clear_log();
void set_time();
void change_pass();*/

#endif	/* MAIN_H */

