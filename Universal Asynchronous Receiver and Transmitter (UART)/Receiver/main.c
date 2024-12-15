/*
Ruben Padilla 7/31/2024

Description: MSP432 uses ADC in P5.0 to detect motion (laps) via IR sensors and types the current lap on the terminal.
             UART is using a function to send the current lap in P1.2 and P1.3
Player 1 laps=a         Player 1 heart monitor=e
Player 2 laps=b         Player 2 heart monitor=f
Player 3 laps=c         Player 3 heart monitor=g
Player 4 laps=d         Player 4 heart monitor=h

UARTA0: P1.2/1.3 is the UBS to GUI
UARTA1:  P2.2/2.3 is Heart rates
UARTA2: P3.2/3.3 is for the Laps
UARTA3: P9.6/9.7 is Heart rates
*/
#include <msp.h>
#include "stdio.h"
    int q,w,x,y;
    char t[20];
void delay(int ms)
    {
        int i;
        for(i= 0; i<ms; i++)//clock frequency
        {

            __delay_cycles(3000); //3MHz clock
        }
    }
void sendStringA0(char *str0); //function for Tx
void sendStringA2(char *str2); //function for Tx
void UARTA0()
{
    // Utilizing UARTA0 (TX: P1.3 & RX P1.2)

    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UAR
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A0->BRW = 19;                                                  // Baudrate width, SMClock/16/DR -> 3000000/16/9600 = 19.53125
    EUSCI_A0->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16; // 19.53125 - 19 = 0.53125 * 16 = 8.5, round up to 9
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;                             // clear reset bit

    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    EUSCI_A0->IFG &= ~EUSCI_A_IE_RXIE;

    // Nested Vectored Interrupt Controller
    NVIC->ISER[0] = 1 << (EUSCIA0_IRQn & 31);
}
void UARTA1()
{
    // Utilizing UARTA1 (TX: P2.3 & RX P2.2)

    P2->SEL0 |= BIT2 | BIT3;
    P2->SEL1 &= ~(BIT2 | BIT3);

    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A1->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UAR
    EUSCI_A1->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A1->BRW = 19;                                                  // Baudrate width, SMClock/16/DR -> 3000000/16/9600 = 19.53125
    EUSCI_A1->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16; // 19.53125 - 19 = 0.53125 * 16 = 8.5, round up to 9
    EUSCI_A1->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;                             // clear reset bit

    EUSCI_A1->IE |= EUSCI_A_IE_RXIE;
    EUSCI_A1->IFG &= ~EUSCI_A_IE_RXIE;

    // Nested Vectored Interrupt Controller
    NVIC->ISER[0] = 1 << (EUSCIA1_IRQn & 31);
}

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
void UARTA3()
{
    // Utilizing UARTA3 (TX: P9.7 & RX P9.6)

    P9->SEL0 |= BIT6 | BIT7;
    P9->SEL1 &= ~(BIT6 | BIT7);

    // Enhanced Universal Serial Control Interface = EUSCI
    EUSCI_A3->CTLW0 = EUSCI_A_CTLW0_SWRST;                               // Clear previous configuration of UAR
    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;                        // Select SMClock, no parity, 1 stop bit, 8 bits, LSB
    EUSCI_A3->BRW = 19;                                                  // Baudrate width, SMClock/16/DR -> 3000000/16/9600 = 19.53125
    EUSCI_A3->MCTLW = (9 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16; // 19.53125 - 19 = 0.53125 * 16 = 8.5, round up to 9
    EUSCI_A3->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;                             // clear reset bit

    EUSCI_A3->IE |= EUSCI_A_IE_RXIE;
    EUSCI_A3->IFG &= ~EUSCI_A_IE_RXIE;

    // Nested Vectored Interrupt Controller
    NVIC->ISER[0] = 1 << (EUSCIA3_IRQn & 31);
}
void UART_ReceiveCharA0(uint8_t* data0)
{
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG));
    *data0 = EUSCI_A0->RXBUF;
}
void UART_ReceiveCharA2(uint8_t* data)
{
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG));
    *data = EUSCI_A2->RXBUF;
}
void UART_ReceiveCharA1(uint8_t* data2)
{
    while (!(EUSCI_A1->IFG & EUSCI_A_IFG_RXIFG));
    *data2 = EUSCI_A1->RXBUF;
}
void UART_ReceiveCharA3(uint8_t* data1)
{
    while (!(EUSCI_A3->IFG & EUSCI_A_IFG_RXIFG));
    *data1 = EUSCI_A3->RXBUF;
}
void main()
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    UARTA2();  // Laps data
    UARTA0();  //to GUI

    UARTA1(); // Heart Rate set 1
    UARTA3(); // Heart Rate set 2

    //LED
    P2->DIR |=BIT2;
    P2->OUT &= ~BIT2;
    P2->OUT |= BIT2;

    while(1)
    {

        //__sleep();
    }
}
void EUSCIA0_IRQHandler(void)
{
    uint8_t receivedData0;
        for (w=0; w<10; w++)
        {
            UART_ReceiveCharA0(&receivedData0);
            t[w]=receivedData0;
            if(receivedData0==0)
            {
                w=10;
            }

        }

       if(receivedData0 =='z')
       {
           sendStringA2(t);
        for(w=0; w<10; w++)
         {
             P2->OUT |= BIT2;
             delay(500);
             P2->OUT &= ~BIT2;
             delay(500);
         }
       }
}
void EUSCIA2_IRQHandler(void)
{

uint8_t receivedData;
char c[10];

    for (x=0; x<10; x++)
    {
        UART_ReceiveCharA2(&receivedData);
        c[x]=receivedData;
        if(receivedData==0)
        {
            x=10;
        }

    }
    sendStringA0(c);
   printf("got %s\n",c);

}
void EUSCIA1_IRQHandler(void)
{

uint8_t receivedData2;
char b[10];

    for (q=0; q<10; q++)
    {
        UART_ReceiveCharA1(&receivedData2);
        b[q]=receivedData2;
        if(receivedData2==0)
        {
            q=10;
        }

    }
    sendStringA0(b);
printf("%s", b);

}
void EUSCIA3_IRQHandler(void)
{

uint8_t receivedData1;
char h[4]; //5

    for (y=0; y<4; y++)
    {
        UART_ReceiveCharA3(&receivedData1);
        h[y]=receivedData1;
        if(receivedData1==0)
        {
            y=4;
        }

    }
    sendStringA0(h);
printf("%s",h);

}

void sendStringA0(char *str0)
{
    int i; // create variable
    // iterate over the end of the string
    for (i = 0; str0[i] != '\0'; i++)
    {
        while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); // wait until is ready to transmit
        EUSCI_A0->TXBUF = str0[i]; // send character through buffer
    }
            printf("Sent : %s ,", str0);
}
void sendStringA2(char *str2)
{
    int i; // create variable
    i=-1;
    do {
        i++;
        while (!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)); // wait until is ready to transmit
        EUSCI_A2->TXBUF = str2[i]; // send character through buffer
    }while(str2[i] != '\0');
            printf("Sent : %s ,", str2);
}
