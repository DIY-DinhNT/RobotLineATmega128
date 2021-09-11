//============================================== Telecommunications University===============================
// Author : DoNgocTuan	/ Telecommunications tech department                                                        
// Email:   ngoctuansqtt@gmail.com	
// Phone:   0986588017												 		
// Date :   21/01/2016                                                        
// Version: 1.0                                                                                                                                 
// Research: www.hocavr.com                                                           
// Description:  MiniRobocon2016 DH_PLV1
// ============================================== make myselft===================================     

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "myLCD.h"

#define gia_tri_start TCNT0
// khai bao dong co
#define Banh_trai 	OCR1AL//PWM 4 onboard 
#define Banh_phai 	OCR1BL//PWM 5 onboard 
#define Banh_1 	    OCR1CL//PWM 6 onboard 
#define tay_xoay_tr 	OCR3AL//PWM 1 onboard 
#define tay_xoay_ph 	OCR3BL//PWM 2 onboard 
#define Banh_3 	    OCR3CL//PWM 3 onboard 

//tham so



unsigned char a,e,SO_VACH,Bien_sensor,Line_no,Nho_trai=0,Nho_phai=0,Vach_ngang;
unsigned char PWMT,PWMT1,TDT1,TDT2,TDT3,TDT4,TDT5,TDT6,TDT7,TDT8,center,kenyt,kenyp;
unsigned char PWMP,PWMP1,TDP1,TDP2,TDP3,TDP4,TDP5,TDP6,TDP7,TDP8;
//==========================macro set,clear  bit============================//
#ifndef cbi
	#define cbi(port, bit) 	   (port) &= ~(1 << (bit)) // macro cho clear bit
#endif
#ifndef sbi
	#define sbi(port, bit) 	   (port) |=  (1 << (bit)) // macro cho set bit
#endif
//===========================macro read bit,============================//

#ifndef read_bit 
	#define read_bit(port_pin, pin_bit) 	((port_pin >> pin_bit) & 1)  //ok dung
#endif

#ifndef in_Pin_Init
	#define in_Pin_Init(DDRx, DDBx) 	(DDRx) &= ~(1 << (DDBx))    //set pin la "in" or "out"
#endif

#ifndef out_Pin_Init
	#define out_Pin_Init(DDRx, DDBx) 	(DDRx) |=  (1 << (DDBx))
	#endif


#ifndef use_r_up 
	#define use_r_up(port, bit) 	   (port) |=  (1 << (bit))
#endif


//*********************************************Ham main*********************************************************//

int main(void)
{
       DDRA=0xFF;
	   DDRB=0xFF;  
	   DDRC=0; // sua de test  	   
       DDRE=0x3F;
	   DDRD=0XF0;
	   DDRF=0; // dung cho cam bien do duong
	   
	   PORTD=0XFF;
	   PORTA=0xFF;           
       PORTB=0xFF; 
	   PORTC=0xFF;
	   PORTE=0xFF; // Pull all for PWM, INT, Program without error
	   PORTF=0XFF;  // dung cho cam bien do duwong
	
// khai bao timer 1 cho PWM5,6
       	TCCR1A=0xFD;//(WGM13=0, WGM12=0, WGM11=0, WGM10=1) giá tr? TOP là 1 h?ng s?, TOP = 255
       	TCCR1B=0x04; // COMA1=COMA0=COMB1=COMB0// GIA TRI XUNG RA HIGH(COMPARE) LOW(TOP)
    	OCR1AL=0;   //pwm1A  value => PWM4 onboard
    	OCR1BL=0;   //pwm1B    value => PWM5 onboard
        OCR1CL=0;  //=> PWM6 onboard
// khai bao timer 3 cho PWM5,6
       	TCCR3A=0xFD;//(WGM13=0, WGM12=0, WGM11=0, WGM10=1) giá tr? TOP là 1 h?ng s?, TOP = 255
       	TCCR3B=0x04; // COMA1=COMA0=COMB1=COMB0// GIA TRI XUNG RA HIGH(COMPARE) LOW(TOP)
    	OCR3AL=0;   //pwm1A  value => PWM1 onboard
    	OCR3BL=0;   //pwm1B    value => PWM2 onboard
        OCR3CL=0;  //=> PWM3 onboard


//========================DHVT1B==================================//
while(1)
{   
	Banh_trai=0;
	Banh_phai=0;
	Van_toc_cham();
	Do_line(1);
	Banh_trai=0;
	Banh_phai=0;
}

}

// ****************** chuong trinh con********************************//

//================CAC HAM DO LINE============================================//
//===========================================================================//
void Quet_cam_bien_vtrang (void)
	{
        DDRF=0;
		Bien_sensor=PINF;
		if	(Bien_sensor==0xff&&Nho_trai==1)			   	{a=1;e=-8;}					  	//lech toan bo qua trai
		if	(Bien_sensor==0x7f)								{a=2,e=-7;Nho_trai=1,Nho_phai=0;}	//01111111
		if	(Bien_sensor==0x3f)								{a=3,e=-6;Nho_trai=1,Nho_phai=0;}	//00111111
		if	(Bien_sensor==0xbf)								{a=4,e=-5;Nho_trai=0,Nho_phai=0;}	//10111111
		if	(Bien_sensor==0x9f)								{a=5;e=-4;}						//10011111
		if	(Bien_sensor==0xdf)								{a=6;e=-3;}						//11011111
		if	(Bien_sensor==0xcf)								{a=7;e=-2;}						//11001111
		if	(Bien_sensor==0xef)								{a=8;e=-1;}						//11101111
		if	(Bien_sensor==0xe7)								{a=9;e=0;}   						//11100111
		if	(Bien_sensor==0xf7)								{a=10;e=1;}						//11110111
		if	(Bien_sensor==0xf3)								{a=11;e=2;}						//11110011
		if	(Bien_sensor==0xfb)								{a=12;e=3;}						//11111011
		if	(Bien_sensor==0xf9)								{a=13;e=4;}						//11111001
		if	(Bien_sensor==0xfd)								{a=14,e=5;Nho_trai=0,Nho_phai=0;}	//11111101
		if	(Bien_sensor==0xfc)								{a=15,e=6;Nho_trai=0,Nho_phai=1;}	//11111100
		if	(Bien_sensor==0xfe)								{a=16,e=7;Nho_trai=0,Nho_phai=1;}	//11111110
		if	(Bien_sensor==0xff&&Nho_phai==1)				{a=17;e=8;}						//lech toan bo sang phai
	}

void DK_do_duong (void)
	{
		if (a==1)	Banh_trai=0,Banh_phai=0;
		if (a==2)	Banh_trai=PWMT1,Banh_phai=TDP7;
		if (a==3)	Banh_trai=PWMT1,Banh_phai=TDP6;
		if (a==4)	Banh_trai=PWMT1,Banh_phai=TDP5;
		if (a==5)	Banh_trai=PWMT1,Banh_phai=TDP4;
		if (a==6)	Banh_trai=PWMT,Banh_phai=TDP3;
		if (a==7)	Banh_trai=PWMT,Banh_phai=TDP2;
		if (a==8)	Banh_trai=PWMT,Banh_phai=TDP1;
		if (a==9) 	Banh_trai=center,Banh_phai=center;
		if (a==10)	Banh_trai=TDT1,Banh_phai=PWMP;
		if (a==11)	Banh_trai=TDT2,Banh_phai=PWMP;
		if (a==12)	Banh_trai=TDT3,Banh_phai=PWMP;
		if (a==13)	Banh_trai=TDT4,Banh_phai=PWMP1;
		if (a==14)	Banh_trai=TDT5,Banh_phai=PWMP1;
		if (a==15)	Banh_trai=TDT6,Banh_phai=PWMP1;
		if (a==16)	Banh_trai=TDT7,Banh_phai=PWMP1;
		if (a==17)	Banh_trai=0,Banh_phai=0;
	}




void KT_qua_vach_ngang (void)// phien ban cu cho vach den
	{                        // now cho vach trang
		unsigned char tam, i, So_cam_bien;
		Vach_ngang = 0;
		while (1)
		{   
			tam = ~PINF;
			So_cam_bien = 0;
			for (i = 0x01; i> 0; i <<= 1)// kiem tra tung mat bang cach dich bit i
			{
				if (tam & i) So_cam_bien ++;// moi mot mat sang tuong ung gia tri kiem tra voi if=> tang 1/
			}				   
			if (So_cam_bien < 5)	break;
			else   So_cam_bien == 0,Vach_ngang = 1;// neu co >5 cam bien sang=> qua 1 vach
		}
		if(Vach_ngang)	++SO_VACH;
	}



void Nhan_vach (void)
	{
		unsigned char tmp, i, one;
	
		tmp = ~PINF;
		one = 0;

		for (i = 0x01; i > 0; i <<= 1)
		{
			if (tmp & i) one ++;
		}

		if (one > 3)
		{
			Line_no ++;
		}
	}

void Do_line (unsigned char so_line)
	{
 		SO_VACH = 0;
		while (1)
		{
			Quet_cam_bien_vtrang();
			DK_do_duong();
			KT_qua_vach_ngang();
			if(SO_VACH==so_line) break;
		}
		    SO_VACH = 0;
	}


//cac ham van toc

void Van_toc_cham (void)
	{
		PWMT = 255,PWMT1 = 180,TDT1 = 150,TDT2 = 100,TDT3 = 80,TDT4 = 60,TDT5 = 40,TDT6 = 20,TDT7 = 10,TDT8 = 10,center=200;
		PWMP = 255,PWMP1 = 180,TDP1 = 150,TDP2 = 100,TDP3 = 80,TDP4 = 60,TDP5 = 40,TDP6 = 20,TDP7 = 10,TDP8 = 10;	
	}

