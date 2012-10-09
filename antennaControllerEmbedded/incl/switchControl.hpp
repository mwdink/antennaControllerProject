/*****************************************************************************/
/** \file
switchControl.hpp

!PURPOSE:
!SUBSYSTEM:
!DEPENDENCIES, LIMITATIONS & DESIGN NOTES:
!REVISION:
    \verbatim
    $File:  $
    $Change:  $
    $Revision: $
    $DateTime:  $
    $Author:  $
    \endverbatim

<em></em>

*//***************************************************************************/
#ifndef SWITCHCONTROL_HPP_
#define SWITCHCONTROL_HPP_
/*----------------------------- Nested includes -----------------------------*/
#include "mobileAntController.hpp"
#include "realSwitches.hpp"

class switchControlHsm;

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
enum switchControlState
{
    SWITCH_CONTROL_HSM_NULL = 0,
    SWITCH_CONTROL_HSM_INITIAL,
    SWITCH_CONTROL_HSM_IDLE,
    SWITCH_CONTROL_HSM_UP,
    SWITCH_CONTROL_HSM_DOWN,
};

enum motorSwitchState
{
    MOTOR_SWITCH_DOWN = -1,
    MOTOR_SWITCH_RELEASED = 0,
    MOTOR_SWITCH_UP = 1
};

enum resetCountSwitchState
{
    RESET_SWITCH_DOWN = 0,
    RESET_SWITCH_RELEASED = 1,
};

/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
extern switchControlHsm *pSwitchControlObj;

/*-------------------------------- Prototypes -------------------------------*/

class switchControlHsm : public QP::QHsm
{
public:
    switchControlHsm();
    ~switchControlHsm();

protected:
    static QP::QState initial(switchControlHsm *me, QP::QEvt const *e);
    static QP::QState operationalState(switchControlHsm *me, QP::QEvt const *e);
    static QP::QState idleState(switchControlHsm *me, QP::QEvt const *e);
    static QP::QState upPushedState(switchControlHsm *me, QP::QEvt const *e);
    static QP::QState downPushedState(switchControlHsm *me, QP::QEvt const *e);

    switchControlState m_currentSwitchControlState;
};





#endif /* SWITCHCONTROL_HPP_ */


