#ifndef DELAY_H
#define	DELAY_H

#define FCY 8000000UL
#include <xc.h>
#include <libpic30.h>
#include <stdio.h>
#include <stdlib.h> 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

void delay_ms(uint16_t time_ms);

#endif	/* DELAY_H */
