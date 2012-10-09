/*
 * motorInterface.hpp
 *
 *  Created on: Sep 13, 2012
 *      Author: dinkem1
 */
#ifndef MOTORINTERFACE_HPP_
#define MOTORINTERFACE_HPP_

/*----------------------------- Nested includes -----------------------------*/
#include <stdint.h>
#include "qp_port.h"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
/*------------------------------- Prototypes --------------------------------*/
class MotorInterface
{
public:
    virtual void initializeMotorHardware(void) = 0;
    virtual void runMotorUp(void) = 0;
    virtual void runMotorDown(void) = 0;
    virtual void setMotorIdle(void) = 0;
};
#endif /* MOTORINTERFACE_HPP_ */
