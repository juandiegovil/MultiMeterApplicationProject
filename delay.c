/*
 * File:   delay.c
 * Author: Hunter Scott, Kameshwara Sekar, Juan Villarreal
 *
 * Created on October 20, 2020, 2:05 PM
 */


#define FCY 8000000UL
#include <xc.h>
#include <libpic30.h>
#include <math.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <errno.h>

// User header files
#include "IOs.h"
#include "ChangeClk.h"
#include "delay.h"

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

void delay_ms(uint16_t time_ms)	   //function written for lab driver project 3
{   
    //start of T2CON configuration
    T2CONbits.TON = 1;
    T2CONbits.TSIDL = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0;
    //end of T2CON configuration
    
    //start of Timer 2 interrupt Configuration
    IPC1bits.T2IP = 7;   //Sets bits 14, 13, and 12 (give timer 2 the highest priority)
    IEC0bits.T2IE = 1;   //setting Interrupt enable control Register 0
    IFS0bits.T2IF = 0;   //setting up the interrupt status register 0
    
    //end of Timer 2 interrupt configuration
    
    //Start of computing PR2
    
    PR2 = time_ms / 0.0625;             //32kHz clk
    //end of computing PR2
    
    IFS1bits.CNIF = 0;
    
    Idle();
}	
	
//Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void)
{   
    IFS0bits.T2IF=0;
    T2CONbits.TON=0;
}
