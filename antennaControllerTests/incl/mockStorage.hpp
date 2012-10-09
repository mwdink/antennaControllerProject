/*
 * mockStorage.hpp
 *
 *  Created on: Oct 5, 2012
 *      Author: dinkem1
 */

#ifndef MOCKSTORAGE_HPP_
#define MOCKSTORAGE_HPP_

#include "realStorage.hpp"
#include "EEPROM.h"

extern int8_t mockEEProm[];
const uint8_t maxStorageSizeInBytes = 200;

class mockStorage : public RealStorage
{
public:
    mockStorage();
    ~mockStorage(){};

    void initializeTestStorage(void);
    void setOffset(uint16_t newOffset);
};

mockStorage::mockStorage()
{
    initializeTestStorage();
}

void
mockStorage::initializeTestStorage(void)
{
    for(int i = 0; i < maxStorageSizeInBytes+20; ++i)
    {
        mockEEProm[i] = 0xff;
    }
}

void
mockStorage::setOffset(uint16_t newOffset)
{
	m_Offset = newOffset;
}

#endif /* MOCKSTORAGE_HPP_ */
