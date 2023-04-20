/*
 * File:   IOs.c
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

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

void IOinit()
{
    AD1PCFG = 0xFFFF;       //Set all pins to digital
    AD1PCFGbits.PCFG5 = 0;  //Set pin AN5 to analog
    AD1PCFGbits.PCFG11 = 0; //Set pin AN11 to analog
    
    /*UART Setup*/
    TRISBbits.TRISB15 = 0;
    REFOCONbits.ROEN = 1;
    REFOCONbits.ROSSLP = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = 0b0000;
    
    /*GPIO Setup*/
    TRISAbits.TRISA4 = 1;   //makes GPIO RA4 as an input
    TRISAbits.TRISA2 = 1;   //makes GPIO RA2 as an input
    TRISBbits.TRISB4 = 1;   //makes GPIO RB4 as an input
    TRISAbits.TRISA3 = 1;   //makes GPIO RA3 as an input
    TRISBbits.TRISB13 = 1;  //makes GPIO RB13 as an input
    //TRISBbits.TRISB8 = 0;   //makes GPIO RB8 as an output     //RB8 not used in app 2
    
    CNPU1bits.CN0PUE = 1;   //enables pull up resistor on RA4/CN0
    CNPU1bits.CN1PUE = 1;   //enables pull up resistor on RB4/CN1
    CNPU2bits.CN30PUE = 1;  //enables pull up resistor on RA2/CN30
    
    CNEN1bits.CN0IE = 1;    //Enable CN interrupts on RA4/CN0
    CNEN1bits.CN1IE = 1;    //Enable CN interrupts on RB4/CN1
    CNEN2bits.CN30IE = 1;   //Enable CN interrupts on RA2/CN30
    
    IPC4bits.CNIP = 2;      //Set priority bits to 2 on CN interrupt
    IEC1bits.CNIE = 1;      //Turn on interrupt enable 
    IFS1bits.CNIF = 0;      //Clear CN interrupt flag
    
    return;
}

void IOcheck() {
    
    float ADC_out;
    
    if (IFS1bits.CNIF == 1) {
        
        while (PORTAbits.RA4 == 1 && PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1) {    //RA2 is pressed
            ADC_out = do_ADC(5);                     //Get ADC value from do_ADC
            
            Disp2String("\rVoltmeter Voltage =");
            Disp2Dec(ADC_out);                       //Display decimal value of ADC_out
            Disp2String("V     ");
        }
        while (PORTAbits.RA4 == 0 && PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1) {    //RA4 is pressed
            ADC_out = do_ADC(11);                     //Get ADC value from do_ADC
       
            //ADC_out = 1000*ADC_out / (3 - ADC_out);   //Calculation not currently working
            
            Disp2String("\rOhmmeter Resistance =");
            //Currently this just displays voltage
            Disp2Dec(ADC_out);                      //Display decimal value of ADC_out
            Disp2String("ohms ");
        }
        while (PORTAbits.RA4 == 1 && PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0) {    //RB4 is pressed
            Idle();
        }
        
        return;
    }
    
    
}