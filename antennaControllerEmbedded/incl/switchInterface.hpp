/*
 * switchInterface.hpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dinkem1
 */
#ifndef SWITCHINTERFACE_HPP_
#define SWITCHINTERFACE_HPP_

/*----------------------------- Nested includes -----------------------------*/
#include <stdint.h>
#include "qp_port.h"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
extern const uint8_t interrupt0;


/*------------------------------ Declarations -------------------------------*/
/*------------------------------- Prototypes --------------------------------*/
class SwitchInterface
{
public:
    virtual int16_t getMotorControlSwitches(void) = 0;
    virtual int16_t getResetCountSwitch(void) = 0;
    virtual void    initializeSwitchHardware(void) = 0;
#if 0
    virtual void toggleTestPin(void) = 0;
#endif

};
#endif /* SWITCHINTERFACE_HPP_ */
