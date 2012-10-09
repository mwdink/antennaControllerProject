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

protected:
    uint8_t switchUp;
    uint8_t switchDown;
    uint8_t resetCountSwitch;

};



#endif /* REALSWITCHES_HPP_ */
