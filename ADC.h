/* 
 * File:   ADC.h
 * Author: Hunter Scott
 *
 * Created on November 21, 2020, 12:32 PM
 */

//#ifndef ADC_H
#define	ADC_H

#define FCY 8000000UL
#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h> 

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

uint16_t do_ADC(uint8_t input_bit);

//#endif	/* ADC_H */

