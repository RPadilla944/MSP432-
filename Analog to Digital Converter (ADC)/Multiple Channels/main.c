#include "msp.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

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

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    ADC();

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
