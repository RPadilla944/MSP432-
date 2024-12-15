#include "msp.h"
#include "lcdLib.h"
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    lcdInit(); // Initialize the LCD
    lcdClear();

        P3->DIR |= BIT6;  //Start Button
        P3->OUT &= ~BIT6;
        P3->OUT |= BIT6;

        P5->DIR |= BIT0; // Stop Button
        P5->OUT &= ~BIT0;
        P5->OUT |= BIT0;


   //SysTick Configuration
    SysTick->CTRL = 0x05;     // CLKSRC | ENABLE, interrupt disable
    SysTick->VAL = 1 - 1;     // clear value register
    SysTick->LOAD = 3000 - 1; // set at 1ms

    lcdSetText("Stopwatch      ", 5, 0);
    int hour=0, min=0, sec=0, millisec=0;
    while (1)
    {
        if (P3->IFG& BIT6)
        {                             // Start stop-watch
        SysTick->CTRL &= ~(0x02); // Disable interrupt
        char buffer[16];
        sprintf(buffer, "%2u:%2u:%2u:%2u", hour, min, sec, millisec);
        lcdSetText(buffer, 1, 1);
            if (millisec++>=999)
            {                         // check after 1 second
                millisec=0; // clear millisecond
                if (sec++>=59)
                {                    // check after a minute
                    sec=0; // clear seconds
                    if (min++>=59)
                    {                    // check after an hour
                        min=0; // clear minutes
                        if (hour++>=23)
                        {                     // check after 24
                            hour=0; // clear hours
                        }
                    }
                }
            }
        }
        if (P5->IFG& BIT0)  // End stopwatch
        {
        SysTick->CTRL |= (0x02); // Enable interrupt
        }
        if (P3->IFG& BIT6)
               {                             // Start stop-watch
               SysTick->CTRL &= ~(0x02); // Disable interrupt
               }
        P3->IFG &= ~(P3->IFG); // clear flag
        P5->IFG &= ~(P5->IFG); // clear flag
    }
    P3->IFG &= ~(P3->IFG); // clear flag
    P5->IFG &= ~(P5->IFG); // clear flag
}



