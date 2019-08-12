#define ADC14CTL0     *adc14_control_register_0
#define ADC14CTL1     *adc14_control_register_1
#define ADC14MCTL0    *adc14_memory_control_register_0
#define ADC14MEM0     *adc14_memory_register_0
#define ADC14IFGR0    *adc14_interrupt_flag_0_register
#define ADC14IER0     *adc14_interrupt_enable_0_register
#define ADC14IER1     *adc14_interrupt_enable_1_register
# include "msp.h"
# include <stdio.h>
long adcRead   = 0;
long middleVal = 0;
void main(void)
   {

   // This function is probably a prerequisite to getting the ADC to work correctly.  It did not
   // fix the ADC "no result" problem, so I am commenting it out.
//   Clock_Init48MHz() ;

   // The watchdog timer is designed to prevent the code from getting stuck somewhere.  If the code fails to
   // reset this time periodically, it will cause the process to burst into flames.  I don't want to deal
   // with this now, so I'll just shut it off.
   WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // See section 17 of the MSP432 Tech Ref manual

   // Title line verifies that the code has started to execute.
   printf("\r\n\n\nMSP 432 ADC to LED Demo\n\n") ;

   // Configure P4.6 to be in ADC mode, not GPIO.  The analog input will come from "channel" 6, which is somehow
   // associated with port4, bit 6.  This selects the "secondary" function for this pin.
   P4SEL1 |= 0x40;
   P4SEL0 |= 0x40;
   P2SEL0 = 0 ;  // This sets all 8 bits in the "port 2 sel 0 register" to zero.
   P2SEL1 = 0 ;  // Likewise for the sel 1 register.
   P2DIR = 1 ;  // Configures bit 0 of Port 2 to be an output while all the other bits will be inputs.



   // Diagnostic printing...
   printf("Set up ADC for input from channel 6\n") ;

   // Set up the ADC.  These bits are all defined in the MSP432P4xx Technical Reference Manual...
   ADC14CTL0  = 0x00000000 ;    // Disable the ADC14 before changing any values.
   ADC14CTL0 &= 0xFFFFFFFD ;    // Note that in this bit mask, only bit 1 is a zero.
   ADC14CTL0 |= 0x00000010 ;    // ADC14 on
   ADC14CTL0 |= 0x04000000 ;    // Source signal from the sampling timer       ****

   // You figure out this one
   ADC14CTL1 =  0x00000000 ;    // ADC14MEM0, 14-bit, ref on, regular power
   //ADC14CTL1 =  0xXXXXXXXX ;
   ADC14MCTL0 = 0x00000087 ;    //  0 to 3.3V, channel A7
   ADC14IER0 =  0 ;             // no interrupts
   ADC14IER1 =  0 ;             // no interrupts
   ADC14CTL0 |= 0x00000002 ;    // enable    But doesn't the core have to be on? (bit 4).  Original code.
  // P6SEL.0 = 1;
   //P6SEL.1 = 1;
   while(1){
       adcRead = 0x1FFF & ADC14MEM0;
       if(adcRead > middleVal)
           P2OUT = 1;
       else
           P2OUT = 0;



   }
}
