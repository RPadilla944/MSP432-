#include "msp.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
char message[]="hello";
void UARTA2() // UART 3.2 & 3.3
{
    // PINS UART Ports
        P3->SEL0 |= BIT2 | BIT3;    // Enable SEL0 special functions for P2.2 and P2.3
        P3->SEL1 &= ~(BIT2 | BIT3); // Clear SEL1 special functions for P2.2 and P2.3

    // UART
        EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UART by setting reset
        EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
        EUSCI_A2->BRW = 19;                                                  // Baudrate width, SMClo/16/DR -> 3000000/16/9600 = 19.53125
        EUSCI_A2->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS | EUSCI_A_MCTLW_OS16); // 19.53125 - 19 = 0.53125 * 16 = 8.5, round up to 9
        EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);                          // clear reset bit
     //Rx
        EUSCI_A2->IE |= EUSCI_A_IE_RXIE;
        EUSCI_A2->IFG &= ~EUSCI_A_IE_RXIE;

      // Nested Vectored Interrupt Controller
        NVIC->ISER[0] = 1 << (EUSCIA2_IRQn & 31);
}
void sendString(char *str); //function for Tx
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    UARTA2();
  
  // P1.1 Switch
    P1->DIR &= ~BIT1;
    P1->REN |= BIT1;
    P1->OUT |= BIT1;

    __enable_irq();                         // enable global interrupts
    while (1)
    {
      if(!(P1->IN&BIT4))
      {
        sendString(message);
      }  
    }
}
void sendString(char *str)
{
    int i; // create variable
    i=-1;
    do {
        i++;
        while (!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)); // wait until is ready to transmit
        EUSCI_A2->TXBUF = str[i]; // send character through buffer
    }while(str[i] != '\0');
            printf("Sent : %s ,", str);
}
