/*****************************************************************************/
/** \file
realDisplay.hpp

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
#ifndef REALDISPLAY_HPP_
#define REALDISPLAY_HPP_
/*----------------------------- Nested includes -----------------------------*/
#include "stdint.h"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
class LiquidCrystal;

/*----------------------- Declarations (externs only) -----------------------*/
extern LiquidCrystal *pDisplay;

/*------------------------------ Declarations -------------------------------*/
/*------------------------------- Prototypes --------------------------------*/
class RealDisplay
{
public:
    RealDisplay();
    virtual ~RealDisplay();


protected:

};

#endif /* REALDISPLAY_HPP_ */
