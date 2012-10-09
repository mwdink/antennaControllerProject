/*****************************************************************************/
/** \file
    switchInterface.hpp

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
#ifndef REALSWITCHES_HPP_
#define REALSWITCHES_HPP_

/*----------------------------- Nested includes -----------------------------*/
#include "switchInterface.hpp"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/* these are all used as digital GPIO */
extern const uint8_t switchUp;
extern const uint8_t switchDown;
extern const uint8_t resetCountSwitch;

/* this needs to be an external interrupt supported pin */
extern const uint8_t reedRelay;

/*------------------------------ Declarations -------------------------------*/
/*------------------------------- Prototypes --------------------------------*/
class RealSwitches: public SwitchInterface
{
public:
    RealSwitches();
    virtual ~RealSwitches(){;}

    int16_t getMotorControlSwitches(void);
    int16_t getResetCountSwitch(void);
    void    initializeSwitchHardware(void);

#if 0
    void toggleTestPin(void);
#endif

private:
};



#endif /* REALSWITCHES_HPP_ */
