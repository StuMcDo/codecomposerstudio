#include "msp.h"

#include <stdio.h>
/*volatile long T32LOAD1    = (volatile long*)0x4000C000/*TA1CTL     = (volatile long*)0x40000400;*/
/*volatile long T32VALUE1   = /*TA1CCTL0   = (volatile long*)0x40000402;*/
/*volatile long T32CONTROL1 = /*TA1CCTL1   = (volatile long*)0x40000404;*/
/*volatile long T32INTCLR1  = /*TA1CCTL2   = (volatile long*)0x40000406;*/
/*volatile long T32RIS1     = /*TA1CCTL3   = (volatile long*)0x40000408;*/
/*volatile long T32MIS1     = /*TA1CCTL4   = (volatile long*)0x4000040A;*/
/*volatile long T32BGLOAD1  = /*TA1R       = (volatile long*)0x40000410;*/
/*volatile long /*TA1CCR0    = (volatile long*)0x40000412;*/
/*volatile long *TA1CCR1    = (volatile long*)0x40000414;*/
/*volatile long *TA1CCR2    = (volatile long*)0x40000416;*/
/*volatile long *TA1CCR3    = (volatile long*)0x40000418;*/
/*volatile long *TA1CCR4    = (volatile long*)0x4000041A;*/
/*volatile long *TA1IV      = (volatile long*)0x4000042E;*/
/*volatile long *TA1EX0     = (volatile long*)0x40000420;*/
/*long upCount = xxxx;
int a;
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	P3SEL0 = 0;
	P3SEL1 = 0;
	P3DIR  |= BIT2;
	P3DIR  &= ~BIT0;
while(1)
{
    a=P3IN&BIT0;
    P3OUT=
            a;
}



}
