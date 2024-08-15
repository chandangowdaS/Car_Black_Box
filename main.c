#include <xc.h>
#include "main.h"

//char time[10] = "00:00:00";
unsigned char clock_reg[3];
unsigned char time[9];
signed int only=1,field_change=0;
signed int field[3];

unsigned char log_data[17]="";

char event[14][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "G5", " C", "VL" ,"DL", "CL", "ST", "CP"}; //all event stored

char menu[5][17] = {"View Log        ", "Download Log    ", "Clear Log       ", "Set Time        ", "Change Pass     "}; //Menu options

unsigned int evi = 0,mni=0,arrow=0x80;//event index

unsigned char pass[] = "0000";

unsigned char user_pass[4];

unsigned char re_user_pass[4];

unsigned int speed, lap=0,one=0,first_time=0,pos_time=2;

signed int var=0;

unsigned int flag = 1,up=0,check=1,passflag=1,attempt=3,pre_evi,shortpress11=0,shortpress12=0,longpress11=0,longpress12=0,enter=9,lpaccess=0,newp=0,overflowflag=0; 

unsigned long int passdelay=0,passndelay=0,nokeypressdelay=0,press11=0,press12=0,cdelay=0;

int rem_time=120,once=0,onetime=0;

//void (*fptr[5])()={view_log, download_log, clear_log, set_time, change_pass};

char read_pass[5];

void clear_reset(void){
    up=0;
    check=1;
    passflag = 1;
    passndelay=0;
    flag=1;
    enter=9;
    CLEAR_DISP_SCREEN;  //clear screen
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void read_pass_eeprom(void){
    for(int i=0;i<4;i++){
        read_pass[i]=read_internal_eeprom(200+i);
    }
}

void write_log_eeprom(void){
    unsigned int hr = ((time[0]-'0')<<4)|(time[1]-'0');
    unsigned int evi1 = evi>>2;
    unsigned int evi0 = evi&0x03;
    unsigned int min_evi1 = (((time[3]-'0')*10)+(time[4]-'0')) | (evi1<<6);
    unsigned int sec_evi0 = (((time[6]-'0')*10)+(time[7]-'0')) | (evi0<<6);
    unsigned int eeprom_log_write[4] = { hr, min_evi1, sec_evi0, speed};
    
    for(int i=0;i<4;i++){
        write_internal_eeprom((lap*4)+i, eeprom_log_write[i]);
    }
    
    if(lap++==10){
        overflowflag=1;
        lap=0;
    }
}

void read_log_eeprom(void){
    /*clcd_putch((var / 4) + '0', LINE2(0));  //log value divide by 0 add by 1  0 ->9
    clcd_putch((read_internal_eeprom(var) >> 4) + '0', LINE2(2));
    clcd_putch((read_internal_eeprom(var) & 0x0F) + '0', LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch((read_internal_eeprom(var+1) & 0x3F) / 10 + '0', LINE2(5));
    clcd_putch((read_internal_eeprom(var+1) & 0x3F) % 10 + '0', LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch((read_internal_eeprom(var+2) & 0x3F) / 10 + '0', LINE2(8));
    clcd_putch((read_internal_eeprom(var+2) & 0x3F) % 10 + '0', LINE2(9));
    clcd_print(event[((read_internal_eeprom(var+1) >> 6) << 2)|(read_internal_eeprom(var+2) >> 6)], LINE2(11));
    clcd_putch(read_internal_eeprom(var+2) / 10 + '0', LINE2(14));
    clcd_putch(read_internal_eeprom(var+2) % 10 + '0', LINE2(15));*/
    
    log_data[0]= var/4 + '0';   //log value divide by 0 add by 1  0 ->9
    log_data[1]= ' ';
    log_data[2]= (read_internal_eeprom(var) >> 4) + '0';
    log_data[3]= (read_internal_eeprom(var) & 0x0F) + '0';
    log_data[4]= ':';
    log_data[5]= ((read_internal_eeprom(var+1) & 0x3F) / 10) + '0';
    log_data[6]= ((read_internal_eeprom(var+1) & 0x3F) % 10) + '0';
    log_data[7]= ':';
    log_data[8]= ((read_internal_eeprom(var+2) & 0x3F) / 10) + '0';
    log_data[9]= ((read_internal_eeprom(var+2) & 0x3F) % 10) + '0';
    log_data[10]= ' ';
    log_data[11]= event[((read_internal_eeprom(var+1) >> 6) << 2)|(read_internal_eeprom(var+2) >> 6)][0];
    log_data[12]= event[((read_internal_eeprom(var+1) >> 6) << 2)|(read_internal_eeprom(var+2) >> 6)][1];
    log_data[13]= ' ';
    log_data[14]= (read_internal_eeprom(var+3) / 10) + '0';
    log_data[15]= (read_internal_eeprom(var+3) % 10) + '0';
    log_data[16]='\0';
}

static void init_config(void) {
    //write_external_eeprom(200, '0');
    //write_external_eeprom(201, '0');
    //write_external_eeprom(202, '0');
    //write_external_eeprom(203, '0');
    
    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_timer0();
    TMR0IE = 0; //disabling timer at start
	GIE = 1;
    init_i2c();
	init_ds1307();
    read_pass_eeprom(); //read pass from EEPROM
    init_uart(); //UART initialization 
}

void main(void) {
    unsigned char key;
    
    init_config();
    
    get_time(); //at start display its initial time
    clcd_print(event[evi], LINE2(10)); //event start should always be on
    write_log_eeprom(); //write initial data to EEPROM

    while (1) {
        
        get_time(); //every time update TIME
        
        if (flag==1) {
            key = read_switches(STATE_CHANGE); //to detect event
            
            speed = read_adc(CHANNEL4) / MAX_SPEED; //to covert in range 0-99
            clcd_print("  TIME   EVNT SP", LINE1(0)); //Dashboard line 1

            clcd_print(time, LINE2(0)); //Real time

            clcd_putch((speed / 10) + '0', LINE2(14)); //Speed
            clcd_putch((speed % 10) + '0', LINE2(15)); //Speed

            //key = read_switches(STATE_CHANGE); //to detect event

            if (key == MK_SW1) { //collision
                evi = 8;
                write_log_eeprom();
            } 
            else if (key == MK_SW2) { //upshift
                if(evi==8){
                    evi=1;
                }
                if (++evi > 7) {
                    evi = 7;
                }
                write_log_eeprom();
            } 
            else if (key == MK_SW3) { //downshift
                if(evi==8){
                    evi=1;
                }
                if (--evi < 1) {
                    evi = 1;
                }
                write_log_eeprom();
            }
            if(evi==8){
                clcd_print(event[evi], LINE2(10));
            }
            else{
                clcd_print(event[evi], LINE2(10));
            }
            pre_evi=evi;
            
            if(key==MK_SW11 || key==MK_SW12){
                CLEAR_DISP_SCREEN;
                flag=0;
            }
        }
        
        else if(flag==0){
            
            if(key==ALL_RELEASED){
                nokeypressdelay++;
            }
            
            if(up!=4){
                if(nokeypressdelay==10000){  //5 seconds delay if no key is pressed
                    evi=pre_evi;
                    nokeypressdelay=0;
                    flag=1;
                }
                if(passdelay<=1250){
                    passdelay++;
                    clcd_print("Enter Password", LINE1(1));
                    clcd_putch('_', LINE2(up));
                }
                else if(passdelay<=2500){
                    passdelay++;
                    clcd_putch(' ', LINE2(up));
                }
                else{
                    passdelay=0;
                }
            }
            
            key = read_switches(STATE_CHANGE); //to detect password
            
            if (up < 4) {  
                check=1;
                if (key == MK_SW11) {
                    nokeypressdelay=0;
                    clcd_putch('*', LINE2(up));
                    user_pass[up++] = '0';
                } 
                else if (key == MK_SW12) {
                    nokeypressdelay=0;
                    clcd_putch('*', LINE2(up));
                    user_pass[up++] = '1';
                }
            }
            else if(up==4){  //Check password
                if(check){
                    for (int i = 0; i < 4; i++) {
                        if (read_pass[i] != user_pass[i]) {
                            passflag = 0;
                            attempt--;
                            break;
                        }
                    }
                    check=0;
                    CLEAR_DISP_SCREEN;
                }           
                if (passflag == 1) {  //Success
                    flag=2;
                    CLEAR_DISP_SCREEN;
                } 
                else{
                    if (attempt == 0) { //If all attempts lost
                        if(rem_time==120){
                            TMR0IE=1;
                            CLEAR_DISP_SCREEN;
                        }
                        
                        clcd_print("ACCESS BLOCKED..", LINE1(0));
                        clcd_print(" REM TIME : ", LINE2(0));
                        clcd_putch(rem_time/10/10 + '0', LINE2(12));
                        clcd_putch(rem_time/10%10 + '0', LINE2(13));
                        clcd_putch(rem_time%10 + '0', LINE2(14));
                        
                        if(rem_time==0){
                            TMR0IE=0;
                            CLEAR_DISP_SCREEN;
                            attempt=3;
                            flag=0;
                            rem_time=120;
                        }
                    } 
                    else {  //Attempts remaining
                        clcd_print("  TRY AGAIN !? ", LINE1(0));
                        clcd_print(" ATTEMPT REM:  ", LINE2(0));
                        clcd_putch(attempt + '0', LINE2(14));
                        if(passndelay++==500){  //display number of attempts left
                            up++;
                        }
                    }
                }
            }
            else {
                CLEAR_DISP_SCREEN;  //clear screen
                evi=pre_evi;
                up=0;
                check=1;
                passflag = 1;
                passndelay=0;
                //passdelay=0;
                //key=ALL_RELEASED;
            }
            
        }
        
        else if(flag==2){
            if(onetime==0){
                clcd_putch(0x7E,arrow);
                clcd_print(menu[mni], LINE1(2));
                clcd_print(menu[mni+1], LINE2(2));
            }
            key = read_switches(LEVEL_CHANGE);
            //detect switch
            if(key==MK_SW11){
                ++press11; //short press
            }
            else if(press11>0 && press11<750){
                shortpress11=1;
                press11=0;
            }
            else if(press11>1500){
                longpress11=1;
                press11=0;
            }
            
            if(key==MK_SW12){
                ++press12; //short press
            }
            else if(press12>0 && press12<750){
                shortpress12=1;
                press12=0;
            }
            else if(press12> 1500){
                longpress12=1;
                press12=0;
            }
            //Scrolling menu logic for short press
            if(arrow==UP && shortpress11){
                if(mni--<=1){
                    mni=0;
                }
                shortpress11=0;
            }
            else if(arrow==UP && shortpress12){
                clcd_print("  ",arrow);
                arrow=DOWN;
                shortpress12=0;
            }
            else if(arrow==DOWN && shortpress11){
                clcd_print("  ",arrow);
                arrow=UP;
                shortpress11=0;
            }
            else if(arrow==DOWN && shortpress12){
                if(mni++>=3){
                    mni=3;
                }
                shortpress12=0;
            }
            //entering the options for long press
            if(longpress11){
                if(arrow==UP){
                    enter=mni+0;
                }
                else if(arrow==DOWN){
                    enter=mni+1;
                }
                lpaccess=1;
                longpress11=0;
                CLEAR_DISP_SCREEN;
                if(enter==2){
                    one=1;
                }
            }
            else if(longpress12){
                //lpaccess=1;
                enter=9;
                longpress12=0;
                flag=1;
                CLEAR_DISP_SCREEN;
            }
            //entering the options
            if(lpaccess){
                onetime++;
                if(onetime==1){
                    ;
                }
                if(enter==0){
                    evi=9;
                    write_log_eeprom();
                    flag=3;  //view log
                }
                else if(enter==1){
                    flag=4;  //Download log
                    evi=10;
                    write_log_eeprom();
                }
                else if(enter==2){
                    if(one==1){
                        clcd_print("Clear log!", LINE1(3));
                        clcd_print("SUCCESSFULLY", LINE2(2));
                        lap=0;
                        overflowflag=0;
                        evi=11;
                        write_log_eeprom();
                    }
                    if(++cdelay==500){
                        one=0;
                        cdelay=0;
                        clear_reset();
                    }
                }
                else if(enter==3){
                    clcd_print("Set Time", LINE1(0));
                    if(key==ALL_RELEASED){
                        flag=5;
                    }
                }
                else if(enter==4){
                    //clcd_print("Change pass", LINE1(0));
                    evi=13;
                    write_log_eeprom();
                    if(key==ALL_RELEASED){
                        flag=6;  //change password
                        first_time=1;
                        
                    }
                }
                else if (enter==9){
                    flag=2;
                    enter=9;
                    onetime=0;
                    CLEAR_DISP_SCREEN;
                    lpaccess=0;
                    evi=pre_evi;
                    arrow==UP;
                    mni=0;
                }
            }
        }
        else if(flag==3){
            clcd_print("View log", LINE1(4));
            read_log_eeprom();
            clcd_print(log_data,LINE2(0)); //printing log data in CLCD
            key = read_switches(LEVEL_CHANGE);
            //detect switch
            if(key==MK_SW11){
                ++press11; //press
            }
            else if(press11>0 && press11<750){
                shortpress11=1;
                press11=0;
            }
            
            if(key==MK_SW12){
                ++press12; //press
            }
            else if(press12>0 && press12<750){
                shortpress12=1;
                press12=0;
            }
            else if(press12> 1500){
                longpress12=1;
                press12=0;
                
            }
            if(shortpress11==1){
                shortpress11=0;
                var=var+4;
            }
            else if(shortpress12==1){
                shortpress12=0;
                var=var-4;
            }
            if(var>=40){
                var=36;
            }
            else if(var<=0){
                var=0;
            }
            if(longpress12==1){
                /*CLEAR_DISP_SCREEN;  //clear screen
                up=0;
                check=1;
                passflag = 1;
                passndelay=0;
                flag=1;*/
                clear_reset();
                var=0;
            }
        }
        else if(flag==4){
            clcd_print("Download log", LINE1(0));
            if(overflowflag==1){
                for(var=0;var<var;var+=4){
                    read_log_eeprom();
                    if(puts(log_data)==0){
                        clcd_print("SUCESSFULL!", LINE2(0));
                    }
                    puts("\n\r");
                }
            }
            else{
                for(int k=0;k<lap;k++,var+=4){
                    read_log_eeprom();
                    if(puts(log_data)==0){
                        clcd_print("SUCESSFULL!", LINE2(0));
                    }
                    puts("\n\r");
                }
            }
            CLEAR_DISP_SCREEN;  //clear screen
            up=0;
            enter=9;
            //check=1;
            //passflag = 1;
            //passndelay=0;
            flag=1;
            var=0;
               
        }
        else if(flag==5){
            
            if(only==1){
                only=0;
                pos_time=2; //default hour blink
                field[0] = ((time[0]-'0')*10+(time[1]-'0'));  //hour
                field[1] = ((time[3]-'0')*10+(time[4]-'0'));  //min
                field[2] = ((time[6]-'0')*10+(time[7]-'0'));  //sec
            }
            /*clcd_putch(field[0]/10 + '0', LINE2(2));
            clcd_putch(field[0]%10 + '0', LINE2(3));
            clcd_putch(':', LINE2(4));
            clcd_putch(field[1]/10 + '0', LINE2(5));
            clcd_putch(field[1]%10 + '0', LINE2(6));
            clcd_putch(':', LINE2(7));
            clcd_putch(field[2]/10 + '0', LINE2(8));
            clcd_putch(field[2]%10 + '0', LINE2(9));*/
            
            //blink
            if(++passdelay<=500){
                clcd_putch(' ', LINE2(pos_time));
                clcd_putch(' ', LINE2(pos_time+1));
            }
            else if(passdelay<=1000){
                clcd_putch(field[0]/10 + '0', LINE2(2));
                clcd_putch(field[0]%10 + '0', LINE2(3));
                clcd_putch(':', LINE2(4));
                clcd_putch(field[1]/10 + '0', LINE2(5));
                clcd_putch(field[1]%10 + '0', LINE2(6));
                clcd_putch(':', LINE2(7));
                clcd_putch(field[2]/10 + '0', LINE2(8));
                clcd_putch(field[2]%10 + '0', LINE2(9));
            }
            else{
                passdelay=0;
            }
            
            
            
            key = read_switches(LEVEL_CHANGE);
            //detect switch
            if(key==MK_SW11){
                ++press11; //press
            }
            else if(press11>0 && press11<750){
                shortpress11=1;
                press11=0;
            }
            else if(press11> 1500){
                longpress11=1;
                press11=0;
                
            }
            
            if(key==MK_SW12){
                ++press12; //press
            }
            else if(press12>0 && press12<750){
                shortpress12=1;
                press12=0;
            }
            else if(press12> 1500){
                longpress12=1;
                press12=0;
                
            }
            if(shortpress11==1){
                shortpress11=0;
                if(field_change==0){
                    if(++field[field_change]>=12){
                        field[field_change]=12;
                    }
                }
                else{
                    if(++field[field_change]>=59){
                        field[field_change]=59;
                    }
                } 
            }
            else if(shortpress12==1){
                shortpress12=0;
                if(--field[field_change]<=0){
                    field[field_change]=0;
                }
            }
            else if(longpress11==1){
                longpress11=0;
                
                //update clock reg value by storing hr min sec in bcd format
                write_ds1307(HOUR_ADDR, ((field[0]/10)<<4)|(field[0]%10));
                write_ds1307(MIN_ADDR, ((field[1]/10)<<4)|(field[1]%10));
                write_ds1307(SEC_ADDR, ((field[2]/10)<<4)|(field[2]%10));
                
                //event triggering
                evi=12;
                write_log_eeprom();
                enter=9;
                CLEAR_DISP_SCREEN;
                flag=1;
            }
            else if(longpress12==1){
                longpress12=0;
                //change field
                pos_time+=3;
                if(++field_change==3){
                    field_change=0;
                    pos_time=2;
                }
            }
        }
        else if(flag==6){
            if(key==ALL_RELEASED && first_time){
                first_time=0;
                newp=1;
                up=0;
            }
            if(up!=4){
                if(passdelay<=1250){
                    passdelay++;
                    clcd_putch('_', LINE2(up));
                }
                else if(passdelay<=2500){
                    passdelay++;
                    clcd_putch(' ', LINE2(up));
                }
                else{
                    passdelay=0;
                }
            }
            
            if(newp==1){
                clcd_print("Enter New Pass", LINE1(0));
                if (up < 4) {  
                    //check=1;
                    if (key == MK_SW11) {
                        clcd_putch('*', LINE2(up));
                        user_pass[up++] = '0';
                    } 
                    else if (key == MK_SW12) {
                        clcd_putch('*', LINE2(up));
                        user_pass[up++] = '1';
                    }
                }
                else if(up==4){
                    up=0;
                    newp=2;
                    CLEAR_DISP_SCREEN;
                }
            }
            else if(newp==2){
                clcd_print("ReEnter Password", LINE1(0));
                if (key == MK_SW11) {
                    clcd_putch('*', LINE2(up));
                    re_user_pass[up++] = '0';
                } 
                else if (key == MK_SW12) {
                    clcd_putch('*', LINE2(up));
                    re_user_pass[up++] = '1';
                }
                if(up==4){
                    newp=3;
                    up=0;
                    CLEAR_DISP_SCREEN;
                }
            }
            else if(newp==3){
                if(check){
                    for (int i = 0; i < 4; i++) {
                        if (re_user_pass[i] != user_pass[i]) {
                            passflag = 0;
                            break;
                        }
                    }
                    check=0;
                }
                if(passflag==1){
                    clcd_print("Password Matched", LINE1(0));
                    clcd_print("                 ", LINE2(0));
                    if(++cdelay==500){
                        write_internal_eeprom(200, re_user_pass[0]);
                        write_internal_eeprom(201, re_user_pass[1]);
                        write_internal_eeprom(202, re_user_pass[2]);
                        write_internal_eeprom(203, re_user_pass[3]);
                        read_pass_eeprom();
                        flag=1;
                        newp=0;
                        clear_reset();
                    }
                }
                else{
                    clcd_print("Password No Match", LINE1(0));
                    clcd_print("                 ", LINE2(0));
                    if(++cdelay==500){
                        flag=1;
                        newp=0;
                        clear_reset();
                    }
                }    
            }
            key = read_switches(STATE_CHANGE); //to detect password
        }
    }
}