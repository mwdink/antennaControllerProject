/*****************************************************************************/
/** \file
    realMotor.hpp

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
#ifndef REALMOTOR_HPP_
#define REALMOTOR_HPP_

/*----------------------------- Nested includes -----------------------------*/
#include "motorInterface.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/* these are all used as digital GPIO */
extern const uint8_t motorUp;
extern const uint8_t motorDown;

/*------------------------------ Declarations -------------------------------*/
/*------------------------------- Prototypes --------------------------------*/
class RealMotor: public MotorInterface
{
public:
    RealMotor();
    virtual ~RealMotor(){;}

    void initializeMotorHardware(void);
    void runMotorUp(void);
    void runMotorDown(void);
    void setMotorIdle(void);

private:
};



#endif /* REALMOTOR_HPP_ */
