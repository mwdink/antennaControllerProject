/*****************************************************************************/
/** \file
    realSwitches.cpp

!PURPOSE:
!SUBSYSTEM:
!DEPENDENCIES, LIMITATIONS & DESIGN NOTES:

!REVISION:
    \verbatim
    $File:  $
    $Change:  $
    $Revision:  $
    $DateTime:  $
    $Author:  $
    \endverbatim

<em> </em>
*//***************************************************************************/
/*----------------------------- Nested includes -----------------------------*/
#include "realSwitches.hpp"
#include "switchControl.hpp"
#include "WProgram.h"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/

/* these are all used as digital GPIO */
const uint8_t switchUp      = 14;       // A0 Arduino analog port 0
const uint8_t switchDown    = 15;       // A1 Arduino analog port 1
const uint8_t resetCountSwitch  = 18;   // A4 Arduino analog port 4



//not a user switch but supported in this file
/* this needs to be an external interrupt supported pin */
const uint8_t reedRelay =   2;      // Arduino digital pin 2
//note: interrupt 0 is digital pin 2
const uint8_t interrupt0 = 0;
/*---------------------------------- Functions ------------------------------*/


/*!Function         RealSwitches::RealSwitches
*   \param
*   \return
*   \par Purpose    ctor
*/
RealSwitches::RealSwitches()
{}


/*!Function         RealSwitches::getMotorControlSwitches
*   \param          void
*   \return         int16_t
*   \par Purpose    query the hardware for the status
*                   of the up/down switches
*/
int16_t
RealSwitches::getMotorControlSwitches(void)
{
    int16_t result = MOTOR_SWITCH_RELEASED;
    static int16_t lastResult = 0xff;
    int8_t up = digitalRead(switchUp);
    int8_t down = digitalRead(switchDown);

    if((0 == up) && (0 == down))
    {
        result = lastResult;
    }
    else if (0 == up)
    {
        result = MOTOR_SWITCH_UP;
    }
    else if(0 == down)
    {
        result = MOTOR_SWITCH_DOWN;
    }

    if(lastResult != result)
    {
        //printf("switchCheck:%d\n", result);
        lastResult = result;
    }
    return result;
}

/*!Function         RealSwitches::getResetCountSwitch
*   \param          void
*   \return         int16_t
*   \par Purpose    query the hardware for the status
*                   of the reset count switch
*/
int16_t
RealSwitches::getResetCountSwitch(void)
{
    int16_t result = RESET_SWITCH_RELEASED;
    if(!digitalRead(resetCountSwitch))
    {
        result = RESET_SWITCH_DOWN;
    }
    return result;
}


/*!Function         RealSwitches::initializeSwitchHardware
*   \param          void
*   \return         void
*   \par Purpose    setup the hardware for use
*/
void
RealSwitches::initializeSwitchHardware(void)
{
    //Set the up down switches for internally pulled up so when the
    //switch is activated, it'll be a LOW reading
    pinMode(switchUp, INPUT);
    digitalWrite(switchUp, HIGH);
    pinMode(switchDown, INPUT);
    digitalWrite(switchDown, HIGH);
    pinMode(resetCountSwitch, INPUT);
    digitalWrite(resetCountSwitch, HIGH);
}


#if 0
/*!Function         RealSwitches::toggleTestPin
*   \param          void
*   \return         void
*   \par Purpose    test function (use o'scope at pin A2)
*/
void
RealSwitches::toggleTestPin(void)
{
    static uint8_t toggle = LOW;
    uint8_t togglePin = 16;         // A2 Arduino analog port 2


    if(LOW == toggle)
    {
        toggle = HIGH;
    }
    else
    {
        toggle = LOW;
    }
    digitalWrite(togglePin, toggle);
}
#endif
