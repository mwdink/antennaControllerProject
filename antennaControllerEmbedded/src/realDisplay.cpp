/*****************************************************************************/
/** \file
realDisplay.cpp

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
/*----------------------------- Nested includes -----------------------------*/
#include "realDisplay.hpp"
#include "LiquidCrystal.h"
#include "stdio.h"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
LiquidCrystal *pDisplay = 0;

static const uint8_t ROW1 = (uint8_t)0;
static const uint8_t ROW2 = (uint8_t)ROW1+1;
static const uint8_t COLUMN0 = (uint8_t)0;
static const uint8_t CONTROLHEADINGCOLUMN = (uint8_t)0;
static const uint8_t ROTATORHEADINGCOLUMN = (uint8_t)13;
static const uint8_t TOTALROWS = (uint8_t)2;
static const uint8_t TOTALCOLUMNS = (uint8_t)16;

/* digital GPIO */
static const uint8_t LCD_RS = (uint8_t)12;      //Arduino digital pin 12
static const uint8_t LCD_ENABLE = (uint8_t)11;  //Arduino digital pin 11
static const uint8_t LCD_D4 = (uint8_t)7;       //Arduino digital pin 7
static const uint8_t LCD_D5 = (uint8_t)6;       //Arduino digital pin 6
static const uint8_t LCD_D6 = (uint8_t)5;       //Arduino digital pin 5
static const uint8_t LCD_D7 = (uint8_t)4;       //Arduino digital pin 4

const char headerMsg[] = "Tic Ring";
//const char errorMsg[] = "Error!";
//const char blank3Msg[] = "   ";
//const char blank10Msg[] = "          ";
//const char twoZeroPad[] = "00 ";
//const char oneZeroPad[] = "0  ";
//const char rotatingMsg[] = "Rotating";
//const char idleMsg[] = "Idle";
//const char rightArrow[] = ">";
//const char leftArrow[] = "<";

/*---------------------------------- Functions ------------------------------*/


/*!Function         RealDisplay::RealDisplay
*   \param
*   \return
*   \par Purpose    ctor
*/
RealDisplay::RealDisplay()
{
    pDisplay = new LiquidCrystal(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
}

/*!Function         RealDisplay::~RealDisplay
*   \param
*   \return
*   \par Purpose    dtor
*/
RealDisplay::~RealDisplay()
{
    if(pDisplay != 0)
    {
        delete pDisplay;
    }
}

