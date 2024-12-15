#include "msp.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

//This code is for the small MSP432R if you have the MSAP432P11y then copy paste code to ne ccs project and use that settings
/*Description: MSP432P4111 uses ADC in P5.0 to detect motion (laps) via IR sensors and types the current lap on the terminal.
             UART1 is using a function to send the current lap in P2.2 (Rx) and P2.3 (Tx)
Player 1 laps=a
Player 2 laps=b
Player 3 laps=c
Player 4 laps=d
*/

uint16_t threshold =16384 / 2; //1.5v

char player1[4];
char player2[4];
char player3[4];
char player4[4];

int number1;
int number2;
int number3;
int number4;
void delay(int ms)
{
    int i;
    for(i= 0; i<ms; i++)//clock frequency
    {

        __delay_cycles(3000); //3MHz clock
    }
}
void ADC()
{
    P5->SEL0 |= (BIT0 | BIT1 | BIT5|BIT4);// player1 = P5.0, player2 = P5.1, player3 = P5.4, player4=P5.5
    P5->SEL1 |= (BIT0 | BIT1 | BIT5|BIT4);

    // SHP = signal from sampling timer, CONSEQ = converts sequence channels
        ADC14->CTL0 |= ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1 | ADC14_CTL0_MSC | ADC14_CTL0_ON;
        ADC14->MCTL[0] |= ADC14_MCTLN_INCH_5;                   //P5.0
        ADC14->MCTL[1] |= ADC14_MCTLN_INCH_4;                   //P5.1
        ADC14->MCTL[2] |= ADC14_MCTLN_INCH_0;                   //P5.5
        ADC14->MCTL[3] |= ADC14_MCTLN_INCH_1 | ADC14_MCTLN_EOS; //P5.4
        ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                   // Select bit resolution
        ADC14->IER0 |= ADC14_IER0_IE2;                          //interrupt


        NVIC->ISER[0] = 1 << ((ADC14_IRQn)&31); // enable NVIC for ADC
}

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

void UART_ReceiveChar(uint8_t* data)
{
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG));
    *data = EUSCI_A2->RXBUF;
}
void sendString(char *str); //function for Tx
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    ADC();
    UARTA2();

    __enable_irq();                         // enable global interrupts
    while (1)
    {
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        __delay_cycles(1000000);

    }
}

void ADC14_IRQHandler(void)
{

    uint16_t adcRaw1 = ADC14->MEM[0];
    uint16_t adcRaw2 = ADC14->MEM[1];
    uint16_t adcRaw3 = ADC14->MEM[2];
    uint16_t adcRaw4 = ADC14->MEM[3];

    char myChar1 = 'a';
    char myChar2 = 'b';
    char myChar3 = 'c';
    char myChar4 = 'd';

   if (adcRaw1 < threshold) // Vcc/2 -> 3.04/2
    {
        number1++;

        sprintf(player1, "%c%d", myChar1, number1);
        sendString(player1);
        printf("ADC Value Player1= %s\n", player1);
        delay(100);

    }
    if (adcRaw2 < threshold) // Vcc/2 -> 3.04/2
    {
        number2++;

        sprintf(player2, "%c%d", myChar2, number2);
        sendString(player2);
        printf("ADC Value Player2= %s\n", player2);
        delay(100);
    }
    if (adcRaw3 < threshold) // Vcc/2 -> 3.04/2
    {
        number3++;

        sprintf(player3, "%c%d", myChar3, number3);
        sendString(player3);
        printf("ADC Value Player3= %s\n", player3);
        delay(100);
    }
    if (adcRaw4 < threshold) // Vcc/2 -> 3.04/2
    {
        number4++;

        sprintf(player4, "%c%d", myChar4, number4);
        sendString(player4);
        printf("ADC Value Player4= %s\n", player4);
        delay(100);
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

void EUSCIA2_IRQHandler(void)
{
int i;
char c[10];
    uint8_t receivedData;
    UART_ReceiveChar(&receivedData);


    c[0]= receivedData;
    c[1]=0;

     printf("receivedData\n");
     printf("Received: %s\n",c);

     if(receivedData=='z')
     {
         printf("receivedData\n");
         printf("Received: %s\n",c);
         number1=0;
         number2=0;
         number3=0;
         number4=0;
     }
}
