/*****************************************************************************/
/** \file
    switchControl.cpp

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

//////////////////////////////////////////////////////////////////////////////
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
/*----------------------------- Nested includes -----------------------------*/
#include "qp_port.h"
//#include "bsp.h"
#include "switchControl.hpp"
//#include "switchInterface.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
static switchControlHsm switchControl;      // the sole instance of the state machine object
QP::QEvt *pSwitchEvent;

/*----------------------- Declarations (externs only) -----------------------*/
extern QP::QHsm *const switchControlHsmObj = &switchControl;

switchControlHsm *pSwitchControlObj = &switchControl;

extern void BSP_addMotorEvent(uint16_t sig);

/*------------------------------ Declarations -------------------------------*/
//QP::QEvt motorEvent = QEVT_INITIALIZER(0);

/*---------------------------------- Functions ------------------------------*/

/*!Function         switchControlHsm::switchControlHsm
*   \param
*   \return
*   \par Purpose    ctor
*/
switchControlHsm::switchControlHsm():
QP::QHsm(Q_STATE_CAST(&switchControlHsm::initial)),
m_currentSwitchControlState(SWITCH_CONTROL_HSM_NULL)
{}


/*!Function         switchControlHsm::~switchControlHsm
*   \param
*   \return
*   \par Purpose    dtor
*/
switchControlHsm::~switchControlHsm()
{}


/*!Function         switchControlHsm::initial
*   \param          switchControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    entry point for the HSM state machine
*/
QP::QState
switchControlHsm::initial(switchControlHsm *me, QP::QEvt const *)
{
    //BSP_display("switch, initial, goto operationalState\n");
    me->m_currentSwitchControlState = SWITCH_CONTROL_HSM_INITIAL;
    return Q_TRAN(&switchControlHsm::operationalState);
}

/*!Function         switchControlHsm::operationalState
*   \param          switchControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose
*/
QP::QState
switchControlHsm::operationalState(switchControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("switch, operationalState-ENTRY\n");
            return Q_HANDLED();
        }
        case Q_INIT_SIG:
        {
            return Q_TRAN(&switchControlHsm::idleState);
        }
        case Q_EXIT_SIG:
        {
        	//BSP_display("switch, operationalState-EXIT;");
            return Q_HANDLED();
        }
        case SWITCH_RELEASED_SIG:
        {
            //BSP_display("switch, operationalState-SWITCH_RELEASED_SIG\n");
            return Q_TRAN(&switchControlHsm::idleState);
        }
    }
    return Q_SUPER(&QHsm::top);
}

/*!Function         switchControlHsm::idleState
*   \param          switchControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    idle state handler for switch control HSM
*/
QP::QState
switchControlHsm::idleState(switchControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("switch, idleState-ENTRY\n");
            me->m_currentSwitchControlState = SWITCH_CONTROL_HSM_IDLE;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG:
        {
            //BSP_display("switch, idleState-EXIT\n");
            return Q_HANDLED();
        }
        case SWITCH_UP_PUSHED_SIG:
        {
            //BSP_display("switch, idleState-SWITCH_UP_PUSHED_SIG\n");
            return Q_TRAN(&switchControlHsm::upPushedState);
        }
        case SWITCH_DOWN_PUSHED_SIG:
        {
            //BSP_display("switch, idleState-SWITCH_DOWN_PUSHED_SIG\n");
            return Q_TRAN(&switchControlHsm::downPushedState);
        }
    }
    return Q_SUPER(&switchControlHsm::operationalState);
}

/*!Function         switchControlHsm::upPushedState
*   \param          switchControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    handler for UP PUSHED state
*/
QP::QState
switchControlHsm::upPushedState(switchControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("switch, upPushedState-ENTRY\n");
            BSP_addMotorEvent(MOTOR_UP_SIG);
            me->m_currentSwitchControlState = SWITCH_CONTROL_HSM_UP;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG:
        {
            //BSP_display("switch, upPushedState-EXIT\n");
            BSP_addMotorEvent(MOTOR_IDLE_SIG);
            return Q_HANDLED();
        }
        case SWITCH_UP_PUSHED_SIG:
        {
            //BSP_display("switch, upPushedState-SWITCH_UP_PUSHED_SIG\n");
            return Q_HANDLED();
        }
        case SWITCH_DOWN_PUSHED_SIG:
        {
            //BSP_display("switch, upPushedState-SWITCH_DOWN_PUSHED_SIG\n");
            return Q_TRAN(&switchControlHsm::idleState);
        }
    }
    return Q_SUPER(&switchControlHsm::operationalState);
}

/*!Function         switchControlHsm::downPushedState
*   \param          switchControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    handler for DOWN PUSHED state
*/
QP::QState
switchControlHsm::downPushedState(switchControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("switch, downPushedState-ENTRY\n");
            BSP_addMotorEvent(MOTOR_DOWN_SIG);
            me->m_currentSwitchControlState = SWITCH_CONTROL_HSM_DOWN;
            return Q_HANDLED();
        }
        case Q_EXIT_SIG:
        {
            //BSP_display("switch, downPushedState-EXIT\n");
            BSP_addMotorEvent(MOTOR_IDLE_SIG);
            return Q_HANDLED();
        }
        case SWITCH_UP_PUSHED_SIG:
        {
           // BSP_display("switch, downPushedState-SWITCH_UP_PUSHED_SIG\n");
            return Q_TRAN(&switchControlHsm::idleState);
        }
        case SWITCH_DOWN_PUSHED_SIG:
        {
            //BSP_display("switch, downPushedState-SWITCH_DOWN_PUSHED_SIG\n");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&switchControlHsm::operationalState);
}
