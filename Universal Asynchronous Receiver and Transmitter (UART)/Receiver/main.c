/*
UARTA2: P3.2/3.3 
*/
#include <msp.h>
#include "stdio.h"
    int i;
void UARTA2()
{
    // Utilizing UARTA2 (TX: P3.3 & RX P3.2)

    P3->SEL0 |= BIT2 | BIT3;
    P3->SEL1 &= ~(BIT2 | BIT3);

    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UAR
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A2->BRW = 19;                                                  // Baudrate width, SMClock/16/DR -> 3000000/16/9600 = 19.53125
    EUSCI_A2->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16; // 19.53125 - 19 = 0.53125 * 16 = 8.5, round up to 9
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;                             // clear reset bit

    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;
    EUSCI_A2->IFG &= ~EUSCI_A_IE_RXIE;

    // Nested Vectored Interrupt Controller
    NVIC->ISER[0] = 1 << (EUSCIA2_IRQn & 31);
}

void UART_ReceiveCharA2(uint8_t* data)
{
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG));
    *data = EUSCI_A2->RXBUF;
}
void main()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
  
    UARTA2();  //UART setup
  
    while(1)
    {
        //__sleep();
    }
}

void EUSCIA2_IRQHandler(void)
{

uint8_t receivedData;
char data[10];

    for (i=0; i<10; i++)
    {
        UART_ReceiveCharA2(&receivedData);
        data[i]=receivedData;
        if(receivedData==0)
        {
            i=10;
        }
    }
   printf("got %s\n",data);

}
