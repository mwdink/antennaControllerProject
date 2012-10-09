/*
 * mockSwitchControl.hpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dinkem1
 */

#ifndef MOCKSWITCHCONTROL_HPP_
#define MOCKSWITCHCONTROL_HPP_

class mock_switchControlHsm : public switchControlHsm
{
public:
    mock_switchControlHsm();
    ~mock_switchControlHsm(){};

    switchControlState getSwitchControlState(void);

};


mock_switchControlHsm::mock_switchControlHsm():switchControlHsm(){}

switchControlState
mock_switchControlHsm::getSwitchControlState(void){return m_currentSwitchControlState;}


#endif /* MOCKSWITCHCONTROL_HPP_ */
