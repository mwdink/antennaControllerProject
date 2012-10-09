/*****************************************************************************/
/* main.cpp

!PURPOSE:
    contains all the hardware specific setups, initializations, and
    the primary task loop. stuff in here is generally not testable

!SUBSYSTEM:

!DEPENDENCIES, LIMITATIONS & DESIGN NOTES:

!REVISION:
    $File:  $
    $Change:  $
    $Revision:  $
    $DateTime:  $
    $Author:    $

Copyright (c) 2012 - All rights reserved.
***************************************************************************/
/*----------------------------- Nested includes -----------------------------*/
#include "stdioFixup.hpp"
#include "uart.hpp"
#include "wiring.h"
#include <qp_port.h>
#include "bsp.h"
#include "mobileAntController.hpp"





/*----------------- Symbolic Constants and Macros (defines) -----------------*/
#define BAUD    9600

/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
const uint8_t MAXEVENTS = 3;
uint16_t motorEventSigPool[MAXEVENTS];
uint8_t  motorEventHead = 0;
uint8_t  motorEventTail = 0;
uint16_t switchEventSigPool[MAXEVENTS];
uint8_t  switchEventHead = 0;
uint8_t  switchEventTail = 0;

/*---------------------------------- Functions ------------------------------*/

/*!Function         main
*   \param
*   \return
*   \par Purpose    program start
*/
int main(void)
{
    usartInit(BAUD);
    setupStdIo();

    BSP_init();

    //QP::QF::init();

    for(int8_t i = 0; i < MAXEVENTS; ++i)
    {
        motorEventSigPool[i] = IGNORE_SIG;
        switchEventSigPool[i] = IGNORE_SIG;
    }

    //pMotorControlObj->setMotorInterface(&realMotor);
    pMotorControlObj = (motorControlHsm *)motorControlHsmObj;
    pMotorControlObj->setMotorInterface(&realMotor);
    motorControlHsmObj->init();
    switchControlHsmObj->init();

    QP::QEvt motorEvent = QEVT_INITIALIZER(0);
    QP::QEvt switchEvent = QEVT_INITIALIZER(0);
    pMotorEvent = &motorEvent;
    pSwitchEvent = &switchEvent;

    QP::QF::run();         // run the application, NOTE: QF::run() does not return

    return 0;
}

