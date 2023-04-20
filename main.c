/*
 * File:   main.c
 * Author: Hunter Scott, Kameshwara Sekar, Juan Villarreal
 *
 * Created on November 20, 2020
 */

// MPLAB header libraries
#include <xc.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>

// User header files
#include "ChangeClk.h"
#include "UART2.h"
#include "ADC.h"
#include "IOs.h"

//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config OSCIOFNC = ON

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

//MAIN
void main(void) {
    
    IOinit();
    
    NewClk(32);
    
    while(1)
    {    
        Idle();
    }   
    
    return;
}

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    T2CONbits.TON = 0;  //turn timer off (if it is currently running)
    IOcheck();
    IFS1bits.CNIF = 0;  //Clear Flag
    Nop();
    
    return;
}
