#include "msp.h"
# include <stdio.h>
    volatile long *adc14_control_register_0 =          (volatile long *)0x40012000 ;  // ADCTL0
    volatile long *adc14_control_register_1 =          (volatile long *)0x40012004 ;  // ADCTL1
    volatile long *adc14_memory_control_register_0 =   (volatile long *)0x40012018 ;  // ADCMEMCTL0
    volatile long *adc14_memory_register_0 =           (volatile long *)0x40012098 ;  // ADC14MEM0
    volatile long *adc14_interrupt_flag_0_register =   (volatile long *)0x40012144 ;  // ADC14IFGR0
    volatile long *adc14_interrupt_enable_0_register = (volatile long *)0x4001213C ;  // ADC14IER0
    volatile long *adc14_interrupt_enable_1_register = (volatile long *)0x4001213C ;  // ADC14IER1
#define ADC14CTL0     *adc14_control_register_0
#define ADC14CTL1     *adc14_control_register_1
#define ADC14MCTL0    *adc14_memory_control_register_0
#define ADC14MEM0     *adc14_memory_register_0
#define ADC14IFGR0    *adc14_interrupt_flag_0_register
#define ADC14IER0     *adc14_interrupt_enable_0_register
#define ADC14IER1     *adc14_interrupt_enable_1_register

#define N 300

    long adcRead   = 0;
    long middleVal = 0;
    int samples/*[10]*/;
    int i = 0;
    int j = 0;
    #define BIT16 (1<<16)
    int sampleLast ;

    volatile long * const control_reg =       (volatile long *)0xE000E010 ;  // STCSR
    volatile long * const reload_value_reg =  (volatile long *)0xE000E014 ;  // STRVR
    volatile long * const current_value_reg = (volatile long *)0xE000E018 ;  // STCVR
    volatile const long *calibration_reg =    (volatile long *)0xE000E01C ;  // STCR
    long adc_wait_count;
    long value = 0;
    long value_array[N][2];
    int x = 0;
    int y = 0;
    long frequency = 0.0;


/**
 * main.c
 */
void main(void)
{
    P4SEL1 |= 0x80;
    P4SEL0 |= 0x80;

    P3SEL0 = 0x00;
    P3SEL1 = 0x00;// set up ports
    P3DIR |= 0x40 ;              // P2.4 output (bit 4 is set)



    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;        // stop watchdog timer
       // Configure P4.6 to be in ADC mode, not GPIO.  The analog input will come from "channel" 6, which is somehow
       // associated with port4, bit 6.  This selects the "secondary" function for this pin.
       //P6SEL1 |= 0x2;
       //P6SEL0 |= 0x2;
    //enable Systic timer for timing.
        SysTick -> LOAD = 0xFFFFF; //    STRVR
        SysTick -> VAL = 0x1;     //    STCVR
        SysTick -> CTRL |= BIT0;   // Enable STCSR
       ADC14CTL0  = 0x00000000 ;    // Disable the ADC14 before changing any values.
       ADC14CTL0 &= 0xFFFFFFFD ;    // Note that in this bit mask, only bit 1 is a zero.
       ADC14CTL0 |= 0x00000010 ;    // ADC14 on
       ADC14CTL0 |= 0x04000000 ;    // Source signal from the sampling timer       ****

       // You figure out this one
       ADC14CTL1  =  0x00000000 ;    // ADC14MEM0, 14-bit, ref on, regular power
       ADC14CTL1 |=  0x00000030 ;
       ADC14MCTL0 = 0x00000086 ;    //  0 to 3.3V, channel A7 or A6
       ADC14IER0 =  0 ;             // no interrupts
       ADC14IER1 =  0 ;             // no interrupts
       ADC14CTL0 |= 0x00000002 ;    // enable    But doesn't the core have to be on? (bit 4).  Original code.
       P6SEL0 = 1;
       P6SEL1 = 1;
       i = 0;
       while(1){
           ADC14CTL0 |= 0x00000001 ;
           //adc_wait_count = 0;
           P3OUT = 0b01000000;
           while(ADC14CTL0 & 0x00010000){

           }

           for(j=0;j<100;j++){
           }
           P3OUT = 0b00000000;
           j=0;

           value_array[i][0] = ADC14MEM0;

           //value_array[i][1] = ;   STEP 5

           i++;
           if(i >= N){
               /*
               for(i=1; i<(N-1);i++){
                   if(value_array[i-1][0] < value_array[i][0]){
                       value_array[i][1] = 1;
                   }
                   if(value_array[i-1][0] > value_array[i][0]){
                       value_array[i][1] = -1;
                   }
                   if(value_array[i-1][0] == value_array[i][0]){
                       value_array[i][1] = 0;
                   }

                   if(value_array[i-3][0] < value_array[i][0] && value_array[i-2][0] < value_array[i][0] && value_array[i-1][0] < value_array[i][0]
                   && value_array[i+1][0] < value_array[i][0] && value_array[i+2][0] < value_array[i][0] && value_array[i+3][0] && x != 0 && y == 0){
                     y = i;
                   }
                   if(value_array[i-3][0] < value_array[i][0] && value_array[i-2][0] < value_array[i][0] && value_array[i-1][0] < value_array[i][0]
                   && value_array[i+1][0] < value_array[i][0] && value_array[i+2][0] < value_array[i][0] && value_array[i+3][0] && x == 0){
                     x = i;
                   }


               }
               */

            for(i=0; i<N;i++){
                   if(value_array[i][0] < value_array[i+1][0] && value_array[i][0] < value_array[i+2][0] && value_array[i][0] < value_array[i+3][0] && value_array[i][0] < value_array[i+4][0]){
                       value_array[i][1] = 1;
                   }
                   else if(value_array[i][0] > value_array[i+1][0] && value_array[i][0] > value_array[i+2][0] && value_array[i][0] > value_array[i+3][0] && value_array[i][0] > value_array[i+4][0]){
                       value_array[i][1] = -1;
                   }
                   else{
                       value_array[i][1] = 0;
                   }
               }
            /*
            for(i=0; i < N; i++){
                if(value_array[i][1] == 1 && value_array[i+1][1] == -1 && x != 0 && y == 0){
                    y = i;
                }
                if(value_array[i][1] == 1 && value_array[i+1][1] == -1 && x == 0){
                    x = i;
                }

            }
            */
            for(i=1; i < N+1; i++){
                //if((value_array[i][1] == 1 && value_array[i+1][1] == -1) && (value_array[i][1] == 1 && value_array[i+2][1] == -1) && (value_array[i][1] == 1 && value_array[i+3][1] == -1)){
                  //  x++;
                //}
                if(value_array[i-1][1] == 0 && value_array[i][1] == -1){
                   x++;
                }
             }

            //frequency = 1/((200/x)*0.00002742);
            frequency = 1/((N/(x/2))*0.000578);

               //frequency = 1/((y - x)*0.00002742);
               //printf("Frequency is: %ld\n", frequency);
               //i = 0;
               x = 0;
               //y = 0;
               i=0;
           }



       }
}


