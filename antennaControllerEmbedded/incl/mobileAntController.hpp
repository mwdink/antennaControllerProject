/*****************************************************************************/
/** \file
mobileAntController.hpp

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
#ifndef MOBILE_ANT_CONTROLLER_HPP_
#define MOBILE_ANT_CONTROLLER_HPP_
/*----------------------------- Nested includes -----------------------------*/
#include "motorControl.hpp"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
enum QHsmTstSignals
{
    TIMER_SIG = QP::Q_USER_SIG,
    IGNORE_SIG,
    MOTOR_HSM_SIGS,
    MOTOR_UP_SIG,
    MOTOR_DOWN_SIG,
    MOTOR_IDLE_SIG,
    MOTOR_MAGNET_SIG,
    MOTOR_TERMINATE_SIG,
    SWITCH_HSM_SIGS,
    SWITCH_UP_PUSHED_SIG,
    SWITCH_DOWN_PUSHED_SIG,
    SWITCH_RELEASED_SIG,
    MAX_SIG
};

/*----------------------- Declarations (externs only) -----------------------*/
extern QP::QHsm *const switchControlHsmObj;
extern QP::QHsm *const motorControlHsmObj;

extern QP::QEvt *pMotorEvent;
extern QP::QEvt *pSwitchEvent;

extern uint16_t motorEventSigPool[];
extern uint16_t switchEventSigPool[];
extern uint8_t  motorEventHead;
extern uint8_t  motorEventTail;
extern uint8_t  switchEventHead;
extern uint8_t  switchEventTail;
extern const uint8_t MAXEVENTS;

/*------------------------------ Declarations -------------------------------*/
/*-------------------------------- Prototypes -------------------------------*/

// BSP functions to display a message and exit
void BSP_display(char const *msg);
void BSP_exit(void);

//int16_t processSwitches(void);
//void initializeHardware(void);
//void toggleTestPin(void);

#endif /* MOBILE_ANT_CONTROLLER_HPP_ */


