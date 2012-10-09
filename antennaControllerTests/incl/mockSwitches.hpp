/*
 * mockSwitches.hpp
 *
 *  Created on: Oct 5, 2012
 *      Author: dinkem1
 */

#ifndef MOCKSWITCHES_HPP_
#define MOCKSWITCHES_HPP_

#include "realSwitches.hpp"

//#define SWITCH_UP_STATE         0xee
//#define SWITCH_DOWN_STATE       0x55
//#define SWITCH_RELEASED_STATE   0xe5
//#define SWITCH_INIT             0x5e

extern int mockDigitalPins[];
extern uint8_t mockPinModes[];

class mockSwitches : public RealSwitches
{
public:
    mockSwitches();
    ~mockSwitches(){};

    void flipSwitch(uint16_t addr, int value){mockDigitalPins[addr] = value;}
    void resetMockPins(void);
};


mockSwitches::mockSwitches()
{
    resetMockPins();
}

void
mockSwitches::resetMockPins(void)
{
    for(int i = 0; i < 20; ++i)
    {
        mockDigitalPins[i] = 0xffff;
        mockPinModes[i] = 0xff;
    }
}




#endif /* MOCKSWITCHES_HPP_ */
