
#include "msp.h"
#include "stdio.h"
void delay(int ms)
    {
        int i;
        for(i= 0; i<ms; i++)//clock frequency
        {

            __delay_cycles(3000); //3MHz clock
        }
    }
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    // pull down, normal button setup
    P5->DIR &= ~BIT6; // set BIT6 as inputs
    P5->OUT &= ~BIT6; // set BIT6 as LOW for pull-down configuration
    P5->REN |= BIT6;    // enable resistors

    //LEDS
    P4->DIR |= (BIT2 | BIT4 | BIT5);
    P4->OUT &= ~(BIT2 | BIT4 | BIT5);
    P4->OUT |= (BIT2 | BIT4 | BIT5);

    while (1)
    {
        P5->OUT &= ~(BIT2 | BIT4 | BIT5); // LEDS off
        if (P5->IN & BIT6)
        {
          printf("ON!\n");
          P4->OUT |= BIT2;
          delay(500); // 0.5 seconds
          P4->OUT &= ~BIT2;
          delay(500);
          P4->OUT |= BIT4;
          delay(500); // 0.5 seconds
          P4->OUT &= ~BIT4;
          delay(500);
          P4->OUT |= BIT5;
          delay(500); // 0.5 seconds
          P4->OUT &= ~BIT5;
          delay(500);
        }

        __delay_cycles(100000);
    }
}
