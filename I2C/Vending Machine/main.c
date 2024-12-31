#include "msp.h"
#include "lcdLib.h"

/**
 * Assignment #3    Vending Machine Interface
 *
 * LCD screen with the following messages:
 *  -"Press selection to view price"    Print in two successive blocks of words
 *  -"ENJOY A REFRESHING DRINK"     Scroll left from the right edje of display
 *  -"37F"      Centered in the display.
 *
 * Inputs:
 *  -3 Buttons
 *      -Letters: pressing must select from "A" to "F" and back (A,B,C,D,E,F).
 *      -Numbers: pressing must change number from 0 to 9.
 *      -OK:    Makes the selection of the desired product.
 *
 * Output:
 *  Show price of selected Item
 *      "A" Items = "PR $4.00"
 *      "B" Items = "PR $3.25"
 *      "C" Items = "PR $3.25"
 *      "D" Items = "PR $2.25"
 *      "E" Items = "PR $2.25"
 *      "F" Items = "Oops!"
 *
 *  ~ Selection has priority over initial messages
 *  ~ A selection not completed within 10 seconds must be ignored, go back to initial display
 *
 */
int f = 0;
int L = 0;
int N = 0;

//Initialize inputs P1.1, P1.4, P1,5 and interrupts for each button
void Input(void){
    //Input setup pull-up
    P1->DIR = ~0x32;
    P1->REN = 0x32;
    P1->OUT = 0x32;

    // Include an extra interrupt on P1.6 to avoid returning to main until necessary
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1->IES = 0x72;
    P1->IFG = 0;
    P1->IE = 0x72;

    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);   //Enable the interrupt Handler
}

//Set SysTick timer to count 10s after an input
void Clock(void){
    SysTick->CTRL = 0;
    SysTick->LOAD = 3000000 - 1;  //At 3MHz = 1s
    SysTick->VAL = 1;
    SysTick->CTRL = 0x05;
    f=0;
}

//Messages to display when system is "Idle"
void Message (void){
    int i = 15;

    lcdClear();
    lcdSetText("Press selection",0,0);
    lcdSetText("to view price",1,1);
    delay_ms(20000);

    for (i;i>=-23;i--){
        lcdClear();
        lcdSetText("ENJOY A REFRESHING DRINK       ",i,0);  //Extra spaces to avoid random character on the screen
        delay_ms(1000);
    }

    lcdClear();
    lcdSetText("37F",7,0);
    delay_ms(10000);
}

//Function to display the correct letter on the LCD screen
void LetSel(void){
    char Letter[] = "ABCDEF";
    char j = Letter[L%6];
    lcdSetText(&j,7,0);

}

//Function to display the correct number on the LCD screen
void NumSel(void){
    int Number[10] = {0,1,2,3,4,5,6,7,8,9} ;
    int j = Number[N%10];
    lcdSetInt(j,8,0);
}

//Display the price of the Item based on the Letter selected
void Price(void){
    char Letter[] = "ABCDEF";
    char j = Letter[(L)%6];
    lcdClear();

    switch (j){
        case'A':
            lcdSetText("PR $4.00",4,0);
            break;
        case'B':
            lcdSetText("PR $3.25",4,0);
            break;
        case'C':
            lcdSetText("PR $3.25",4,0);
            break;
        case'D':
            lcdSetText("PR $2.25",4,0);
            break;
        case'E':
            lcdSetText("PR $2.25",4,0);
            break;
        case'F':
            lcdSetText("Oops!",5,0);
            break;
    }
    delay_ms(10000);
}

//Read the corresponding button and act accordingly.
//P1.1 = Letters
//P1.4 = Numbers
//P1.5 - OK
void PORT1_IRQHandler(void){

    //Run functions to display the corresponding value on the LCD
    if (f == 0) {
        lcdClear();
        LetSel();
        NumSel();
        lcdSetText("      ",9,0);
    }
    P1->IFG |= BIT6;    //Turn this flag ON to avoid going back to main until necessary

    if (P1->IFG & BIT1){
        L++;
        Clock();
        P1->IFG &= ~BIT1;
    }

    if (P1->IFG & BIT4){
        N++;
        Clock();
        P1->IFG &= ~BIT4;
    }


    if (P1->IFG & BIT5){
        Price();
        SysTick->CTRL = 0;
        P1->IFG &= ~BIT6;
        P1->IFG &= ~BIT5;
        L = 0;
        N = 0;
        f = 0;
    }

    delay_ms(100);

    //Systick Timer counts every second, when done 10 times turns P1.6 flag OFF and goes back to main.
    if ((SysTick->CTRL & 0x00010000)){
        f++;
        if (f==10){
            SysTick->CTRL = 0;
            P1->IFG &= ~BIT6;
            L = 0;
            N = 0;
            f = 0;
        }
    }
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Input();
    lcdInit();
    lcdClear();

    __enable_irq();     //Enable global interrupt

    while(1){
        Message();
    }


}

