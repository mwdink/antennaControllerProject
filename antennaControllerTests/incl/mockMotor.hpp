/*
 * mockHardware.hpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dinkem1
 */

#ifndef MOCKMOTOR_HPP_
#define MOCKMOTOR_HPP_

//#include "motorInterface.hpp"
#include "realMotor.hpp"

#define MOTOR_UP_STATE      0xee
#define MOTOR_DOWN_STATE    0x55
#define MOTOR_IDLE_STATE    0xe5
#define MOTOR_INIT          0x5e


extern int mockDigitalPins[];
extern uint8_t mockPinModes[];

//class mockMotor : public MotorInterface
class mockMotor : public RealMotor
{
public:
    mockMotor();
    ~mockMotor(){};

    void resetMockPins(void);
};

mockMotor::mockMotor()
{
    resetMockPins();
}

void
mockMotor::resetMockPins(void)
{
    for(int i = 0; i < 20; ++i)
    {
        mockDigitalPins[i] = 0xffff;
        mockPinModes[i] = 0xff;
    }
}



#endif /* MOCKMOTOR_HPP_ */
