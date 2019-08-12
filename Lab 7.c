#include "msp.h"
#include <stdio.h>


/**
 * main.c
 */
int i = 0;
int count = 0;
int count2 = 0;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
        P1SEL0 = 0x0;
        P1SEL1 = 0x0;// set up ports
        P3SEL0 = 0x0;
        P3SEL1 = 0x0;
        P1DIR  = 0xC0;
        P3DIR  = 0xC0;
        P1OUT  = 0b01000000;//turn on motors
        P3OUT  = 0b11000000;
        P2DIR  |=  0xD0 ;              // P2.4 output (bit 4 is set)
        P2SEL0 |=  0xD0 ;              // P2.4 Timer0A functions (choose PWM as the source)
        P2SEL1 &= ~0xD0 ;              // P2.4 Timer0A functions
        TIMER_A0->CCTL[0] = 0x0080;    // CCI0 toggle
        TIMER_A0->CCR[0] =  500;      // Period will vary with the clock settings.  You can measure it.
        TIMER_A0->EX0 =     0x0000;    // Divide by 1
        TIMER_A0->CCTL[1] = 0x0040;    // CCR1 toggle/reset
        TIMER_A0->CCTL[3] = 0x0040;
        TIMER_A0->CCTL[4] = 0x0040;
        TIMER_A0->CCR[1] =   250 ;     // CCR1 duty cycle is duty1/period
        TIMER_A0->CCR[3] =   600 ;
        TIMER_A0->CCR[4] =  600;

        TIMER_A0->CTL =     0x0230;    // SMCLK=12MHz, divide by 1, up-down mode


        P7SEL0 = 0x0;
        P7SEL1 = 0x0;


        while(1){
            P7DIR |= 0b00100000;//right motor pwm control
            P7OUT |= 0b00100000;
            while (P7IN == 0b00100000){
                P7DIR &=0b111011111;
                count++;
             //   count2++;


            }
            if(count > 180){
                TIMER_A0->CCR[3] = 250;  //350 ;
            }
            else{
                TIMER_A0->CCR[3] =   100;  // 100 ;
            }

                       P7DIR |= 0b00000100;
                       P7OUT |= 0b00000100;
                       while (P7IN == 0b00000100){
                           P7DIR &=0b111111011;
                           //count++;
                           count2++;


                       }
                       if(count2 > 180){
                                       TIMER_A0->CCR[4] =  250;  //  350 ;
                                   }
                                   else{
                                       TIMER_A0->CCR[4] =  100;  //  100 ;
                                   }


            count = 0;
            count2 = 0;


        }
}
