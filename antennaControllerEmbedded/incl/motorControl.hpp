/*****************************************************************************/
/** \file
    motorControl.hpp

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
#ifndef MOTOR_CONTROL_HPP_
#define MOTOR_CONTROL_HPP_
/*----------------------------- Nested includes -----------------------------*/
#include "mobileAntController.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
class MotorInterface;
class motorControlHsm;

/*-------------------------- Typedefs and structs ---------------------------*/
enum motorDirection
{
    MOTOR_DOWN = 1,
    MOTOR_IDLE = 0,
    MOTOR_UP = -1
};

enum motorControlState
{
    MOTOR_CONTROL_HSM_NULL = 0,
    MOTOR_CONTROL_HSM_INITIAL,
    MOTOR_CONTROL_HSM_IDLE,
    MOTOR_CONTROL_HSM_UP,
    MOTOR_CONTROL_HSM_DOWN,
};

/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
extern motorControlHsm *pMotorControlObj;

/*-------------------------------- Prototypes -------------------------------*/

class motorControlHsm : public QP::QHsm
{
public:
    motorControlHsm();
    ~motorControlHsm();

    void setMotorInterface(MotorInterface *motorInterface);
    int16_t setMotorPulseCount(int16_t newCount);
    int16_t getMotorPulseCount(void);

protected:
    static QP::QState initial(motorControlHsm *me, QP::QEvt const *e);
    static QP::QState operationalState(motorControlHsm *me, QP::QEvt const *e);
    static QP::QState idleState(motorControlHsm *me, QP::QEvt const *e);
    static QP::QState rotateState(motorControlHsm *me, QP::QEvt const *e);
    static QP::QState upState(motorControlHsm *me, QP::QEvt const *e);
    static QP::QState downState(motorControlHsm *me, QP::QEvt const *e);

    int16_t m_pulseCount;               //positional count of magnet pulses
    motorDirection m_motorDirection;    //last direction of motor
    motorControlState currentMotorControlState;

    MotorInterface *m_pMotorInterface;


};





#endif /* MOTOR_CONTROL_HPP_ */


