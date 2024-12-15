#include "msp.h"
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
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P1->DIR &= ~(BIT1 | BIT4);
    P1->REN |= (BIT1 | BIT4);
    P1->OUT |= (BIT1 | BIT4);

    //LEDS
    P2->DIR |= (BIT1 | BIT2);
    P2->OUT &= ~(BIT1 | BIT2);
    P2->OUT |= (BIT1 | BIT2);

    int x,y;
    while(1)
    {
        if(!(P1->IN&BIT4))
        {
            for(x = 0; x<10; x++)
            {
               P2->OUT |= BIT2;
               delay(500); // 0.5 seconds
               P2->OUT &= ~BIT2;
               delay(1000);
            }

        }
        if(!(P1->IN&BIT1))
        {
            for(y = 0; y<10; y++)
            {
              P2->OUT |= BIT1;
              delay(500);
              P2->OUT &= ~BIT1;
              delay(1000);
            }

        }

    }
}
