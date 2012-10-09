/*****************************************************************************/
/** \file
    realMotor.cpp

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
#include "realMotor.hpp"
#include "switchControl.hpp"
#include "WProgram.h"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/

/* these are all used as digital GPIO */
const uint8_t motorUp   =   16;         // A2 Arduino analog port 2
const uint8_t motorDown =   17;         // A3 Arduino analog port 3

/*---------------------------------- Functions ------------------------------*/


/*!Function         RealMotor::RealMotor
*   \param
*   \return
*   \par Purpose    ctor
*/
RealMotor::RealMotor()
{}




/*!Function         RealMotor::initializeMotorHardware
*   \param          void
*   \return         void
*   \par Purpose    setup the hardware for use
*   \note           INPUT_PULLUP not implemented
*/
void
RealMotor::initializeMotorHardware(void)
{
    pinMode(motorUp, OUTPUT);
    digitalWrite(motorUp, HIGH);
    pinMode(motorDown, OUTPUT);
    digitalWrite(motorDown, HIGH);
}

/*!Function         RealMotor::runMotorUp
*   \param          void
*   \return         void
*   \par Purpose    start the motor running upwards
*/
void
RealMotor::runMotorUp(void)
{
    digitalWrite(motorUp, LOW);
    digitalWrite(motorDown, HIGH);
}

/*!Function         RealMotor::runMotorDown
*   \param          void
*   \return         void
*   \par Purpose    start the motor running downwards
*/
void
RealMotor::runMotorDown(void)
{
    digitalWrite(motorUp, HIGH);
    digitalWrite(motorDown, LOW);
}

/*!Function         RealMotor::setMotorIdle
*   \param          void
*   \return         void
*   \par Purpose    stop the motor
*/
void
RealMotor::setMotorIdle(void)
{
    digitalWrite(motorUp, HIGH);
    digitalWrite(motorDown, HIGH);
}


