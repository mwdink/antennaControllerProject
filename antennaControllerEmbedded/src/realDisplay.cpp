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
static const uint8_t COLUMN_PLUS_MINUS = (uint8_t)12;
static const uint8_t COLUMN_COUNT = (uint8_t)13;
static const uint8_t TOTALROWS = (uint8_t)2;
static const uint8_t TOTALCOLUMNS = (uint8_t)16;

/* digital GPIO */
static const uint8_t LCD_RS = (uint8_t)12;      //Arduino digital pin 12
static const uint8_t LCD_ENABLE = (uint8_t)11;  //Arduino digital pin 11
static const uint8_t LCD_D4 = (uint8_t)7;       //Arduino digital pin 7
static const uint8_t LCD_D5 = (uint8_t)6;       //Arduino digital pin 6
static const uint8_t LCD_D6 = (uint8_t)5;       //Arduino digital pin 5
static const uint8_t LCD_D7 = (uint8_t)4;       //Arduino digital pin 4

const char headerMsg[] = "N7WA  Mobile";  //keep this even for ease of centering
const char blankCountMsg[] = "    ";
const char twoZeroPad[] = "00 ";
const char oneZeroPad[] = "0  ";
const char blankDirectionMsg[] = "    ";
const char idleMsg[]  = "Idle";
const char upMsg[]    = " Up ";
const char downMsg[]  = "Down";

#if 0  //2 line, 16 character display
**N7WA  Mobile**
Idle*********000

**N7WA  Mobile**
**Up********-002

**N7WA  Mobile**
Down*********005

#endif




/*---------------------------------- Functions ------------------------------*/


/*!Function         RealDisplay::RealDisplay
*   \param
*   \return
*   \par Purpose    ctor
*/
RealDisplay::RealDisplay()
{}

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

/*!Function         RealDisplay::initializeDisplay
*   \param          void
*   \return         void
*   \par Purpose    configure the display
*/
void
RealDisplay::initializeDisplay(void)
{
    pDisplay = new LiquidCrystal(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

    if(pDisplay)
    {
        pDisplay->begin(TOTALCOLUMNS, TOTALROWS);
    }
}

/*!Function         RealDisplay::displayHeader
*   \param          void
*   \return         void
*   \par Purpose    send the banner to the LCD
*/
void
RealDisplay::displayBanner(void)
{
    if(pDisplay)
    {
        // Print a message to the LCD.
        pDisplay->setCursor((TOTALCOLUMNS - strlen(headerMsg))/2, ROW1);
        pDisplay->print(headerMsg);
    }
}

/*!Function         RealDisplay::displayDirection
*   \param          const char *msg
*   \return         void
*   \par Purpose    update the display into indicate the
*                   antenna motor travel direction
*/
void
RealDisplay::displayDirection(char *msg)
{
    pDisplay->setCursor(COLUMN0, ROW2);
    pDisplay->print(blankDirectionMsg);
    pDisplay->setCursor(COLUMN0, ROW2);
    pDisplay->print(msg);
}


/*!Function         RealDisplay::displayCount
*   \param          int16_t count
*   \return         void
*   \par Purpose    update the display to indicate the
*                   antenna motor position
*/
void
RealDisplay::displayCount(int16_t count)
{
    pDisplay->setCursor(COLUMN_PLUS_MINUS, ROW2);
    pDisplay->print(blankCountMsg);
}


void
RealDisplay::updateDisplay(displayControl command, int16_t data)
{
    switch(command)
    {
    case DISPLAY_CONFIG:
        initializeDisplay();
        break;
    case DISPLAY_BANNER:
        displayBanner();
        break;
    case DISPLAY_UP:
        displayDirection((char *)upMsg);
        break;
    case DISPLAY_DOWN:
        displayDirection((char *)downMsg);
        break;
    case DISPLAY_IDLE:
        displayDirection((char *)idleMsg);
        break;
    case DISPLAY_COUNT:
        displayCount(data);
        break;
    default:
        break;
    }

}
