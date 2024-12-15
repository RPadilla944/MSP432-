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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // clear the secondary functions
    P5->SEL0 &= ~(BIT6);
    P5->SEL1 &= ~(BIT6);

    // pull down, normal button setup
    P5->DIR &= ~BIT6; // set BIT6 as inputs
    P5->OUT &= ~BIT6; // set BIT6 as LOW for pull-down configuration
    P5->REN |= BIT6;    // enable resistors

      // Interrupt Configuration
        P5->IE |= BIT6;     // enable BIT6 as interrupt
        P5->IES &= ~BIT6; // set BIT6 as rising edge, 0->1 or pull-down
        P5->IFG &= ~BIT6; // clear interrupt flag

      NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);   //Enable the interrupt Handler
    // enable global interrupts
    __enable_irq();

    int i=0;
    while(1)
    {
      i=i+1;
      printf("Counting: %d\n",i);
      delay(1000); // 1 second delay
    }
}
void PORT5_IRQHandler(void)
{
    int a;

        if (P5->IFG & BIT6) // set interrupt
        {
          for(a = 0 ; a < 5; a++)
          {
              printf("I am interrupting you 5 times!\n");
          }
        }

       P5->IFG &= ~BIT6; // clear interrupt

}
