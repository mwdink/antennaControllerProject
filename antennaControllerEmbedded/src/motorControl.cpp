/*****************************************************************************/
/** \file
    motorControl.cpp

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
#include "motorControl.hpp"
#include "motorInterface.hpp"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
static motorControlHsm motorControl;      // the sole instance of the state machine object
QP::QEvt *pMotorEvent;

/*----------------------- Declarations (externs only) -----------------------*/
extern QP::QHsm *const motorControlHsmObj = &motorControl;             // the opaque pointer

/*------------------------------ Declarations -------------------------------*/
//motorControlHsm *pMotorControlObj = &motorControl;
motorControlHsm *pMotorControlObj = 0;

/*---------------------------------- Functions ------------------------------*/


/*!Function         motorControlHsm::motorControlHsm
*   \param
*   \return
*   \par Purpose    ctor
*/
motorControlHsm::motorControlHsm():
QP::QHsm(Q_STATE_CAST(&motorControlHsm::initial)),
m_pulseCount(0),
m_motorDirection(MOTOR_IDLE),
currentMotorControlState(MOTOR_CONTROL_HSM_NULL),
m_pMotorInterface(0)
{}


/*!Function         motorControlHsm::~motorControlHsm
*   \param
*   \return
*   \par Purpose    dtor
*/
motorControlHsm::~motorControlHsm()
{}


/*!Function         motorControlHsm::setMotorInterface
*   \param          MotorInterface *motorInterface
*   \return         void
*   \par Purpose    all the user to define the motor interface
*                   to be used (i.e. mock or real)
*/
void
motorControlHsm::setMotorInterface(MotorInterface *motorInterface)
{
    m_pMotorInterface = motorInterface;
}

/*!Function         motorControlHsm::setMotorPulseCount
*   \param          int16_t newCount
*   \return         void
*   \par Purpose    update the pulse count to a new value
*/
void
motorControlHsm::setMotorPulseCount(int16_t newCount)
{
    m_pulseCount = newCount;
}


/*!Function         motorControlHsm::getMotorPulseCount
*   \param          void
*   \return         int16_t
*   \par Purpose    get the current pulse count
*/
int16_t
motorControlHsm::getMotorPulseCount(void)
{
    return m_pulseCount;
}


/*!Function         motorControlHsm::initial
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    entry point for the HSM state machine
*/
QP::QState
motorControlHsm::initial(motorControlHsm *me, QP::QEvt const *)
{
    //BSP_display("initial, goto operationalState\n");
    me->currentMotorControlState = MOTOR_CONTROL_HSM_INITIAL;
    return Q_TRAN(&motorControlHsm::operationalState);
}

/*!Function         motorControlHsm::operationalState
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    top level state for motor control HSM
*/
QP::QState
motorControlHsm::operationalState(motorControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("motor, operationalState-ENTRY\n");
            //BSP_display("obtaining Stored Pulse Count\n");
            BSP_display("motor is IDLE\n");
            me->m_pulseCount = 0;  //initialize pulseCount
            me->m_motorDirection = MOTOR_IDLE; //initialize motor direction
            return Q_HANDLED();
        }
        case Q_INIT_SIG:
        {
            return Q_TRAN(&motorControlHsm::idleState);
        }
        case Q_EXIT_SIG:
        {
            //BSP_display("motor, operationalState-EXIT\n");
            return Q_HANDLED();
        }
        case MOTOR_MAGNET_SIG:
        {
            //BSP_display("motor, operationalState-magnet\n");
            me->m_pulseCount += me->m_motorDirection;
            return Q_HANDLED();
        }
        case MOTOR_TERMINATE_SIG:
        {
            //BSP_display("motor, operationalState-terminate\n");
            BSP_exit();
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm::top);
}


/*!Function         motorControlHsm::idleState
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    idle state handler for motor control HSM
*/
QP::QState
motorControlHsm::idleState(motorControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("motor, idleState-ENTRY\n");
            me->currentMotorControlState = MOTOR_CONTROL_HSM_IDLE;
            //turn off the motor
            me->m_pMotorInterface->setMotorIdle();
            return Q_HANDLED();
        }
        case Q_EXIT_SIG:
        {
            //BSP_display("motor, idleState-EXIT\n");
            return Q_HANDLED();
        }
        case MOTOR_UP_SIG:
        {
            //BSP_display("motor, idleState-MOTOR_UP_SIG\n");
            return Q_TRAN(&motorControlHsm::upState);
        }
        case MOTOR_DOWN_SIG:
        {
            //BSP_display("motor, idleState-MOTOR_DOWN_SIG\n");
            return Q_TRAN(&motorControlHsm::downState);
        }
    }
    return Q_SUPER(&motorControlHsm::operationalState);
}

/*!Function         motorControlHsm::rotateState
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    rotate state handler for motor control HSM
*                   has two sub-states for rotating CW and CCW
*/
QP::QState motorControlHsm::rotateState(motorControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("motor, rotateState-ENTRY\n");
            return Q_HANDLED();
        }

        case MOTOR_IDLE_SIG:
        {
            //BSP_display("motor, rotateState-MOTOR_IDLE_SIG\n");
            return Q_TRAN(&motorControlHsm::idleState);
        }

        case Q_EXIT_SIG:
        {
            //BSP_display("motor, rotateState-EXIT\n");
            BSP_display("stop motor\n");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&motorControlHsm::operationalState);
}

/*!Function         motorControlHsm::upState
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    state handler for motor control HSM
*                   in this state the motor is rotating the contacts up
*                   the antenna coil
*/
QP::QState
motorControlHsm::upState(motorControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("upState-ENTRY\n");
            //BSP_display("motor Direction is UP\n");
            me->m_motorDirection = MOTOR_UP;
            me->currentMotorControlState = MOTOR_CONTROL_HSM_UP;
            //start the motor (up)
            me->m_pMotorInterface->runMotorUp();
            return Q_HANDLED();
        }

        case Q_INIT_SIG:
        {
            BSP_display("start motor (UP)\n");
            return Q_HANDLED();
        }

        case MOTOR_UP_SIG:
        {
            //BSP_display("upState-MOTOR_UP_SIG\n");
            return Q_HANDLED();
        }

        case MOTOR_DOWN_SIG:
        {
            //BSP_display("upState-MOTOR_DOWN_SIG\n");
            return Q_TRAN(&motorControlHsm::idleState);
        }

        case Q_EXIT_SIG:
        {
            //BSP_display("upState-EXIT\n");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&motorControlHsm::rotateState);
}


/*!Function         motorControlHsm::downState
*   \param          motorControlHsm *me
*   \param          QP::QEvt const *
*   \return         QP::QState
*   \par Purpose    state handler for motor control HSM
*                   in this state the motor is rotating the contacts down
*                   the antenna coil
*/
QP::QState
motorControlHsm::downState(motorControlHsm *me, QP::QEvt const *e)
{
    switch (e->sig)
    {
        case Q_ENTRY_SIG:
        {
            //BSP_display("downState-ENTRY\n");
            //BSP_display("motor Direction is DOWN\n");
            me->m_motorDirection = MOTOR_DOWN;
            me->currentMotorControlState = MOTOR_CONTROL_HSM_DOWN;
            //start the motor (down)
            me->m_pMotorInterface->runMotorDown();
            return Q_HANDLED();
        }

        case Q_INIT_SIG:
        {
            BSP_display("start motor (DOWN)\n");
            return Q_HANDLED();
        }

        case MOTOR_UP_SIG:
        {
            //BSP_display("upState-MOTOR_UP_SIG\n");
            return Q_TRAN(&motorControlHsm::idleState);
        }

        case MOTOR_DOWN_SIG:
        {
            //BSP_display("upState-MOTOR_DOWN_SIG\n");
            return Q_HANDLED();
        }

        case Q_EXIT_SIG:
        {
            //BSP_display("downState-EXIT\n");
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&motorControlHsm::rotateState);
}



