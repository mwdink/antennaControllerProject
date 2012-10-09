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
enum displayControl
{
    DISPLAY_CONFIG = 0,
    DISPLAY_BANNER,
    DISPLAY_UP,
    DISPLAY_DOWN,
    DISPLAY_IDLE,
    DISPLAY_COUNT
};

/*------------------------------- Prototypes --------------------------------*/
class RealDisplay
{
public:
    RealDisplay();
    virtual ~RealDisplay();

    void updateDisplay(displayControl command, int16_t data);

protected:
    void initializeDisplay(void);
    void displayBanner(void);
    void displayDirection(char *msg);
    void displayCount(int16_t count);

};

#endif /* REALDISPLAY_HPP_ */




