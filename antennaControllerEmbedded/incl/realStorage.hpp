/*****************************************************************************/
/** \file
realStorage.hpp

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

#ifndef REALSTORAGE_HPP_
#define REALSTORAGE_HPP_
/*----------------------------- Nested includes -----------------------------*/
#include "stdint.h"


/*----------------- Symbolic Constants and Macros (defines) -----------------*/
/*-------------------------- Typedefs and structs ---------------------------*/
/*----------------------- Declarations (externs only) -----------------------*/
/*------------------------------ Declarations -------------------------------*/
class EEPROMClass;

/*------------------------------- Prototypes --------------------------------*/
class RealStorage
{
public:
    RealStorage();
    virtual ~RealStorage();

    void    resetStorage(void);
    int16_t getStoredCount(void);
    void    storeCount(int16_t value);

    uint8_t getMaxStorageSizeInBytes(void){return m_MaxStorageSizeInBytes;}
    uint16_t getOffset(void);

protected:
    int16_t  m_Count;
    uint16_t m_Offset;
    const uint8_t m_MaxStorageSizeInBytes;
    const int16_t m_defaultCount;
    EEPROMClass *m_pStorage;

};

#endif /* REALSTORAGE_HPP_ */
