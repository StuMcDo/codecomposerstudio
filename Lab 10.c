
#include "msp.h"
#include "math.h"
#include <stdio.h>
#define BIT16 (1<<16)
/**
* main.c
*/

int final3 = 0;
int final6 = 0;
int rightSensorTape;
int leftSensorTape;

int startTime = 0;
int endTime = 0;

int testCount = 0;
float error = 0.0;

int rightOutput;
int leftOutput;

int innerRightOutput;
int innerLeftOutput;

int innerIR;

int maxErrorOrangeLeft = 3700;
int maxErrorOrangeRight = 3700;

int baseOrangeLeft = 6155;
int baseOrangeRight = 4432;

int maxErrorBlueLeft = 3800;
int maxErrorBlueRight = 3800;

int baseBlueLeft = 6939;
int baseBlueRight = 5782;

int maxErrorYellowLeft = 4000;
int maxErrorYellowRight = 4100;

int baseYellowLeft = 6911;
int baseYellowRight = 6262;

int maxErrorRedLeft = 4700;
int maxErrorRedRight = 4400;

int baseRedLeft = 7611;
int baseRedRight = 6682;

int maxErrorBlackLeft = 19600;
int maxErrorBlackRight = 18800;

int baseBlackLeft = 7611;
int baseBlackRight = 6682;


float kOrangeRight;
float kOrangeLeft;
float kORangeBackRight;
float kORangeBackLeft;
float kBlueRight;
float kBlueLeft;
float kBlueBackRight;
float kBlueBackLeft;
float kYellowRight;
float kYellowLeft;
float kYellowBackRight;
float kYellowBackLeft;
float kRedRight;
float kRedLeft;
float kRedBackRight;
float kRedBackLeft;
float kBlackRight;
float kBlackLeft;
float kBlackBackRight;
float kBlackBackLeft;



turnRedLed(){
    P2OUT &= (~BIT1);
    P2OUT &= (~BIT2);
    P2OUT |= BIT0 ;
}

turnBlueLed(){
    P2OUT &= (~BIT1);
    P2OUT &= (~BIT0);
    P2OUT |= BIT2 ;
}

turnGreenLed(){
    P2OUT &= (~BIT0);
    P2OUT &= (~BIT2);
    P2OUT |= BIT1 ;
}

int readRightSensor(){
    startTime = SysTick -> VAL;
     while (P7IN &= 0b00000100){
     P7DIR &=0b111111011;
     }
     endTime = SysTick -> VAL;
     return startTime - endTime;
}

/*
 * Black tape centered
 *  -final3 right = 7312 correct at 9000 for final 3 saturation 16100
 *  -final6 left = 9443 correct at 11000 saturation 19000
 */
int readLeftSensor(){
    startTime = SysTick -> VAL;
    while (P7IN &= 0b00100000){
       P7DIR &=0b11011111;
    }
    endTime = SysTick -> VAL;
    return startTime - endTime;
}

readLeftOuterSensor(){
    startTime = SysTick -> VAL;
    while (P7IN &= 0b10000000){
           P7DIR &=0b01111111;
    }
    endTime = SysTick -> VAL;
    return startTime - endTime;
}

readRightOuterSensor(){
    startTime = SysTick -> VAL;
    while (P7IN &= 0b00000001){
           P7DIR &=0b11111110;
    }
    endTime = SysTick -> VAL;
    return startTime - endTime;
}


readInnerLeft(){
    startTime = SysTick -> VAL;
       while (P7IN &= 0b00000010){
              P7DIR &=0b11111101;
       }
       endTime = SysTick -> VAL;
       return startTime - endTime;
}



readInnerRight(){
    startTime = SysTick -> VAL;
       while (P7IN &= 0b00010000){
              P7DIR &=0b11101111;
       }
       endTime = SysTick -> VAL;
       return startTime - endTime;
}

avoidLimits(){
    if(TIMER_A0->CCR[3] < 25){
         TIMER_A0->CCR[3] = 25;
     }
     if(TIMER_A0->CCR[3] > 350){
         TIMER_A0->CCR[3] = 350;
     }
     if(TIMER_A0->CCR[4] < 25){
         TIMER_A0->CCR[4] = 25;
      }
      if(TIMER_A0->CCR[4] > 350){
         TIMER_A0->CCR[4] = 350;
      }
}



void main(void)
{
WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer


P1SEL0 = 0x0;
P1SEL1 = 0x0;// set up ports

P3SEL0 = 0x0;
P3SEL1 = 0x0;

P1DIR = 0xC0;
P1OUT = 0b00000000;//turn on motors

P3DIR = 0xC0;
P3OUT = 0b11000000;

P2DIR |= 0xC7 ; // P2.4 output (bit 4 is set)
P2SEL0 |= 0xD0 ; // P2.4 Timer0A functions (choose PWM as the source)
P2SEL1 &= ~0xD0 ; // P2.4 Timer0A functions

TIMER_A0->CCTL[0] = 0x0080; // CCI0 toggle
TIMER_A0->CCR[0] = 500; // Period will vary with the clock settings. You can measure it.
TIMER_A0->EX0 = 0x0000; // Divide by 1
TIMER_A0->CCTL[1] = 0x0040; // CCR1 toggle/reset
TIMER_A0->CCTL[3] = 0x0040;
TIMER_A0->CCTL[4] = 0x0040;
TIMER_A0->CCR[1] = 250 ; // CCR1 duty cycle is duty1/period
TIMER_A0->CCR[3] = 85 ;
TIMER_A0->CCR[4] = 85;
TIMER_A0->CTL = 0x0230; // SMCLK=12MHz, divide by 1, up-down mode

P7SEL0 = 0x0;
P7SEL1 = 0x0;
//ADC14->MCTL[0] = 0
// Configure SysTick
SysTick -> LOAD = 0xFFFFF; //    STRVR
SysTick -> VAL = 0x1;     //    STCVR
SysTick -> CTRL |= BIT0;   // Enable STCSR

P2DS = 0x7; //set high drive strength

kOrangeRight = (float) 150/abs(maxErrorOrangeRight - baseOrangeRight);
kOrangeLeft = (float) 150/abs(maxErrorOrangeLeft - baseOrangeLeft);
kORangeBackRight = (float) 50/abs(maxErrorOrangeRight - baseOrangeRight);
kORangeBackLeft = (float) 50/abs(maxErrorOrangeLeft - baseOrangeLeft);
kBlueRight = (float) 150/abs(maxErrorBlueRight - baseBlueRight);
kBlueLeft = (float) 150/abs(maxErrorBlueLeft - baseBlueLeft);
kBlueBackRight = (float) 75/abs(maxErrorBlueRight - baseBlueRight);
kBlueBackLeft = (float) 75/abs(maxErrorBlueLeft - baseBlueLeft);
kYellowRight = (float) 150/abs(maxErrorYellowRight - baseYellowRight);
kYellowLeft =  (float) 150/abs(maxErrorYellowLeft - baseYellowLeft);
kYellowBackRight = (float) 75/abs(maxErrorYellowRight - baseYellowRight);
kYellowBackLeft = (float) 75/abs(maxErrorYellowLeft - baseYellowLeft);
kRedRight = (float) 150/abs(maxErrorRedRight - baseRedRight);
kRedLeft =  (float) 150/abs(maxErrorRedLeft - baseRedLeft);
kRedBackRight = (float) 75/abs(maxErrorRedRight - baseRedRight);
kRedBackLeft = (float) 75/abs(maxErrorRedLeft - baseRedLeft);
kBlackRight = (float) 250/abs(maxErrorBlackRight - baseBlackRight);
kBlackLeft = (float) 250/abs(maxErrorBlackLeft - baseBlackLeft);
kBlackBackRight = (float) 75/abs(maxErrorBlackRight - baseBlackRight);
kBlackBackLeft = (float) 75/abs(maxErrorBlackLeft - baseBlackLeft);



 while(1){


     P7DIR |= 0b10110111; //right motor pwm control
     P7OUT |= 0b10110111;

     innerLeftOutput = readInnerLeft();
     innerRightOutput = readInnerRight();
     innerIR = (innerLeftOutput + innerRightOutput)/2;

     rightOutput = readRightSensor();
     leftOutput = readLeftSensor();

     /*
     if(TIMER_A0->CCR[3] < 25){
         TIMER_A0->CCR[3] = 25;
     }
     if(TIMER_A0->CCR[3] > 350){
         TIMER_A0->CCR[3] = 350;
     }
     if(TIMER_A0->CCR[4] < 25){
         TIMER_A0->CCR[4] = 25;
      }
      if(TIMER_A0->CCR[4] > 350){
         TIMER_A0->CCR[4] = 350;
      }
      */


     if(innerIR > 1000 && innerIR < 3800){ // orange
         if(rightOutput < baseOrangeRight){
               error = abs(rightOutput - baseOrangeRight);
               TIMER_A0->CCR[4] = 100 + round(error*kOrangeRight);
               TIMER_A0->CCR[3] = 100 - round(error*kORangeBackRight);

          }
          if(leftOutput < baseOrangeLeft){
               error = abs(leftOutput - baseOrangeLeft);
               TIMER_A0->CCR[3] = 100 + round(error*kOrangeLeft);
               TIMER_A0->CCR[4] = 100 - round(error*kORangeBackLeft);
          }
     }
     if(innerIR >= 3800 && innerIR < 4100){ // blue
        if(rightOutput < baseBlueRight){
               error = abs(rightOutput - baseBlueRight);
               TIMER_A0->CCR[4] = 100 + round(error*kBlueRight);
               TIMER_A0->CCR[3] = 100 - round(error*kBlueBackRight);
               avoidLimits();
          }
          if(leftOutput < baseBlueLeft){
               error = abs(leftOutput - baseBlueLeft);
               TIMER_A0->CCR[3] = 100 + round(error*kBlueLeft);
               TIMER_A0->CCR[4] = 100 - round(error*kBlueBackLeft);
               avoidLimits();
          }
     }
     if(innerIR >= 4100 && innerIR < 4300){ // yellow
        if(rightOutput < baseYellowRight){
               error = abs(rightOutput - baseYellowRight);
               TIMER_A0->CCR[4] = 100 + round(error*kYellowRight);
               TIMER_A0->CCR[3] = 100 - round(error*kYellowBackRight);
               avoidLimits();
          }
          if(leftOutput < baseYellowLeft){
               error = abs(leftOutput - baseYellowLeft);
               TIMER_A0->CCR[3] = 100 + round(error*kYellowLeft);
               TIMER_A0->CCR[4] = 100 - round(error*kYellowBackLeft);
               avoidLimits();
          }

     }
     if(innerIR >= 4300 && innerIR < 6000){ // red
        if(rightOutput < baseRedRight){
               error = abs(rightOutput - baseRedRight);
               TIMER_A0->CCR[4] = 100 + round(error*kRedRight);
               TIMER_A0->CCR[3] = 100 - round(error*kRedBackRight);
          }
          if(leftOutput < baseRedLeft){
               error = abs(leftOutput - baseRedLeft);
               TIMER_A0->CCR[3] = 100 + round(error*kRedLeft);
               TIMER_A0->CCR[4] = 100 - round(error*kRedBackLeft);
          }
     }
     if(innerIR >= 6000 && innerIR < 19000){ // black
         if(rightOutput > baseBlackRight){
               error = abs(rightOutput - baseBlackRight);
               TIMER_A0->CCR[4] = 100 + round(error*kBlackRight);
               TIMER_A0->CCR[3] = 100 - round(error*kBlackBackRight);
          }
          if(leftOutput > baseBlackLeft){
               error = abs(leftOutput - baseBlackLeft);
               TIMER_A0->CCR[3] = 100 + round(error*kBlackLeft);
               TIMER_A0->CCR[4] = 100 - round(error*kBlackBackLeft);
          }
     }

 }
}
