#include "msp.h"
uint16_t threshold = 16384 / 2; // VCC/2 or 1.15V
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
    // Enable ADC Pins
    P5->SEL0 |= BIT0;
    P5->SEL1 |= BIT0;

    // Enable on-board LED
    P2->DIR |= BIT2;     // Bit Set P2.2
    P2->OUT &= ~(BIT2);  // Bit Clear P2.2
    P2->SEL0 &= ~(BIT2); // Bit Clear SEL0 for P2.2
    P2->SEL1 &= ~(BIT2); // Bit Clear SEL1 for P2.2
    // Set-Up for ADC
    ADC14->CTL0 |= ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_0 | ADC14_CTL0_ON; //
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_5;                                 // set ADC channel as A5 = P5.0
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                                // set resolution as a 14 bits resolution
    ADC14->IER0 |= ADC14_IER0_IE0;                                       // interrupt for mem 0

    // enable NVIC for ADC14
    NVIC->ISER[0] |= 1 << ((ADC14_IRQn)&31); // enable nvic

    // enable global interrupts
    __enable_irq();

    while (1)
    {
        __delay_cycles(1000000);                       // delay
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; // start conversion and enable conversion
    }
}
void ADC14_IRQHandler(void)
{
    uint16_t adcRaw = ADC14->MEM[0]; // read adc data

    if (adcRaw > threshold) // Vcc/2 -> 3.04/2
    {
        P2->OUT = BIT2; // turn on BLUE led
    }
    else
    {
        P2->OUT &= ~(BIT2); // turn off LED
    }
}
