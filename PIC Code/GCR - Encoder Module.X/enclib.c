/* Alan Hale III
 * GCRobotics
 *
 * 2013/3/5
 *
 *
 * Encoder Library
 *
 * This library is responsible for setting up Timer 0 as a timer and Timer 1
 *  as a counter.  Timer 1 will be counting the encoder cycles from the motor.
 *  This library is also responsible for returning the current values of the
 *  timers.  Upon a read, a reset to 0 will typically occur.
 */


#include "enclib.h"
#include <pic16f917.h>


// Function that initialises Timer0 as a timer and Timer1 as a counter
void BeginEncoder()
{
    // Timer0 Setup
    //  Timer0 is controlled by the OPTION_REG (shared by WDT)
    //  Two main things are:  TMR0 controlled by internal clock
    //   and Prescaler assigned to WDT (giving TMR0 1:1 prescale)
    //  See page 103 of 917 datasheet for OPTION_REG
    OPTION_REG = 0b10011000;

    // Timer1 Setup
    //  Timer1 is controlled by the T1CON register
    //  Main things are:  no gating, prescale 1:1, external clock
    //   asynchronous, timer on (starts counting encoder stuff right away)
    CMCON1 = 0x02;
    T1CON = 0b00000111;
}


// Function that takes in two pointers and sets one equal to TMR0 and
//  the other equal to TMR1.  The values of TMR0 and TMR1 are then reset.
void EncUpdate(int *t0, int *t1)
{
    *t1 = TMR1;
    *t0 = TMR0;
    TMR1 = 0;
    TMR0 = 0;
}

