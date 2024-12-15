
#include "msp.h"
#include "stdio.h"
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    // pull down, normal button setup
    P5->DIR &= ~BIT6; // set BIT6 as inputs
    P5->OUT &= ~BIT6; // set BIT6 as LOW for pull-down configuration
    P5->REN |= BIT6;    // enable resistors

    while (1)
    {
        P5->OUT &= ~BIT5; // LED is off
        if (P5->IN & BIT6)
        {
          printf("HI!\n");
        }

        __delay_cycles(100000);
    }
}
