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
#include "realStorage.hpp"
#include "EEPROM.h"
#include "stdio.h"

/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
EEPROMClass *pStorage;
const int16_t defaultCount = 0x03e7;  //999 decimal
const int16_t maxStorageSizeInBytes = 200;

/*---------------------------------- Functions ------------------------------*/


/*!Function         RealStorage::RealStorage
*   \param
*   \return
*   \par Purpose    ctor
*/
RealStorage::RealStorage():
m_Count(0),
m_Offset(0),
m_MaxStorageSizeInBytes(maxStorageSizeInBytes)    //100 words
{
    pStorage = new EEPROMClass;
}

/*!Function         RealStorage::~RealStorage
*   \param
*   \return
*   \par Purpose    dtor
*/
RealStorage::~RealStorage()
{
    delete pStorage;
}

/*!Function         RealStorage::resetStorage
*   \param          void
*   \return         void
*   \par Purpose    clear out the area used for
*                   storage of the motor positional counts
*   \note           we're writing int16_t values so increment
*                   appropriately
*/
void
RealStorage::resetStorage(void)
{
    printf("resetStorage\n");
    for(int16_t addr = 0x0000; addr < maxStorageSizeInBytes; addr+=2)
    {
        pStorage->writeInt((int)addr, defaultCount);
    }
    m_Offset = 0;
}

/*!Function         RealStorage::getOffset
*   \param          uint16_t
*   \return         void
*   \par Purpose    return the offset into eeprom where the next
*                   count update should be written
*/
uint16_t
RealStorage::getOffset(void)
{
    return m_Offset;
}

/*!Function         RealStorage::getStoredCount
*   \param          uint16_t
*   \return         void
*   \par Purpose    return the motor positional count value. do this by
*                   reading each value in the storage area (first 100 words
*                   in the eeprom). the actual value we want is the last
*                   valid (!=999)
*/
int16_t
RealStorage::getStoredCount(void)
{
    for(int16_t addr = 0x0000; addr < m_MaxStorageSizeInBytes; addr+=2)
    {
        m_Count = pStorage->readInt((int)addr);
        m_Offset = addr+2; //next place to write next update

        if(m_Count >= defaultCount)
        {
            if(addr > 0)
            {
                m_Offset = addr; //next place to write next update
                addr-=2;          //address of last valid value
                m_Count = pStorage->readInt((int)addr);  //last valid value
            }
            else //defaults
            {
                m_Offset = 0;
                m_Count = 0;
            }
            break;
        }
    }
    printf("getStoredCount, count:%d,  offset:%d\n", m_Count, m_Offset);
    return m_Count;
}

/*!Function         RealStorage::storeCount
*   \param          uint16_t
*   \return         void
*   \par Purpose    save the motor positional count value in eeprom and
*                   update the offset to the next address (valid == 999)
*/
void
RealStorage::storeCount(int16_t value)
{
    if(!(m_Offset < m_MaxStorageSizeInBytes))
    {
        resetStorage();
    }
    printf("storeCount:%d at %d\n", value, m_Offset);
    pStorage->writeInt((int)m_Offset, value);
    m_Offset+=2;
}
