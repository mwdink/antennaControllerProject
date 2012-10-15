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
/*---------------------------------- Functions ------------------------------*/


/*!Function         RealMotor::RealMotor
*   \param
*   \return
*   \par Purpose    ctor
*/
RealMotor::RealMotor()
{
    /* digital GPIO */
    motorUp   =   16;         // A2 Arduino analog port 2
    motorDown =   17;         // A3 Arduino analog port 3
}




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
    digitalWrite(motorUp, LOW);
    pinMode(motorDown, OUTPUT);
    digitalWrite(motorDown, LOW);
}

/*!Function         RealMotor::runMotorUp
*   \param          void
*   \return         void
*   \par Purpose    start the motor running upwards
*/
void
RealMotor::runMotorUp(void)
{
    digitalWrite(motorUp, HIGH);
    digitalWrite(motorDown, LOW);
}

/*!Function         RealMotor::runMotorDown
*   \param          void
*   \return         void
*   \par Purpose    start the motor running downwards
*/
void
RealMotor::runMotorDown(void)
{
    digitalWrite(motorUp, LOW);
    digitalWrite(motorDown, HIGH);
}

/*!Function         RealMotor::setMotorIdle
*   \param          void
*   \return         void
*   \par Purpose    stop the motor
*/
void
RealMotor::setMotorIdle(void)
{
    digitalWrite(motorUp, LOW);
    digitalWrite(motorDown, LOW);
}


