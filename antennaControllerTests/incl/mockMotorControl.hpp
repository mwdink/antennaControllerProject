/*
 * mockMotorControl.hpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dinkem1
 */
#ifndef MOCKMOTORCONTROL_HPP_
#define MOCKMOTORCONTROL_HPP_

#include "qp_port.h"
#include "motorControl.hpp"
#include "stdint.h"

//class mock_motorControlHsm : public motorControlHsm
//{
//public:
//    mock_motorControlHsm();
//    ~mock_motorControlHsm(){};
//
//    motorControlState getMotorControlState(void);
//    int16_t getPulseCount(void);
//    motorDirection getMotorDirection(void);
//
//    motorControlState currentMotorControlState;
//    int16_t m_pulseCount;
//    motorDirection m_motorDirection;
//};

class mock_motorControlHsm : public motorControlHsm
{
public:
    mock_motorControlHsm();
    ~mock_motorControlHsm(){};

    motorControlState getMotorControlState(void);
    int16_t getPulseCount(void);
    motorDirection getMotorDirection(void);
};


mock_motorControlHsm::mock_motorControlHsm():motorControlHsm(){}

motorControlState
mock_motorControlHsm::getMotorControlState(void){return currentMotorControlState;}

int16_t
mock_motorControlHsm::getPulseCount(void){return m_pulseCount;}

motorDirection
mock_motorControlHsm::getMotorDirection(void){return m_motorDirection;}



#endif /* MOCKMOTORCONTROL_HPP_ */
