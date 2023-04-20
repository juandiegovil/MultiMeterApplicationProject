/*
 * File:   ADC.c
 * Author: Hunter Scott, Kameshwara Sekar, Juan Villarreal
 *
 * Created on November 20, 2020, 2:05 PM
 */

#include <xc.h>
#include <libpic30.h>
#include <math.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <errno.h>

// User header files
#include "xc.h"
#include "ADC.h"



uint16_t do_ADC(uint8_t input_bit)	   //function written for lab driver project 5
{   
    uint16_t ADCvalue;
    
    /*ADC Initialization*/
    AD1CON1bits.ADON = 1;         //Turn on ADC
    AD1CON1bits.FORM = 0b00;      //Data output is in integer form
    AD1CON1bits.ASAM = 0;         //Sampling starts when SAMP bit is set to 1
    AD1CON1bits.SSRC = 0b111;     //Internal counter ends sampling and starts conversion
    
    AD1CON2bits.VCFG = 0b000;     //Selects AVDD, AVSS (Supply voltage to PIC) as Vref
    AD1CON2bits.BUFM = 0;         //Buffer configured as 1 16-word
    AD1CON2bits.ALTS = 0;         //Always uses mux A input settings
    AD1CON2bits.CSCNA = 0;        //Set to not scan inputs
    AD1CON2bits.SMPI = 0b0000;    //Interrupts disabled
    
    AD1CON3bits.ADRC = 0;         //Use system clock
    AD1CON3bits.SAMC = 0b11111;   //Slowest sampling time (31*2 / fclk) (1/10 sampling time)
    
    AD1CHSbits.CH0NA = 0;         //Channel 0 negative input is Vr-
    
    //Determine which pin to collect data from
    if (input_bit == 5) {
        AD1CHSbits.CH0SA = 0b0101;    //Channel 0 positive input it AN5
    }
    else if (input_bit == 11) {
        AD1CHSbits.CH0SA = 0b1011;    //Channel 0 positive input it AN11
    }
    
    AD1CSSL = 0;                  //Input scan is disabled
    
    /*ADC Sampling and Conversion*/
    AD1CON1bits.SAMP = 1;        //Start sampling, conversion starts automatically after SSRC and SAMC settings
    while (AD1CON1bits.DONE == 0) {}
    
    ADCvalue = ADC1BUF0;        //ADC output is stored in ADC1BUF0
    AD1CON1bits.SAMP = 0;       //Stop sampling
    AD1CON1bits.ADON = 0;       //Turn off ADC, value stored in ADC1BUF0
    return(ADCvalue);           //returns 10 bit ADC output stored in ADC1BUF0 to calling function
}

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0;
    AD1CON1bits.ADON = 0;
}