/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include "stdio.h"

extern int8_t mockEEProm[];
class EEPROMClass
{
public:
    uint8_t read(int){return 0;}

    int readInt(int addr)
	{
    	int result = 0;
//    	if(addr < 10)
//    	{
//    	printf("y:0x%hhx, 0x%hhx\n",(int8_t)mockEEProm[addr], (int8_t)mockEEProm[addr+1]);
//        }
    	result = (mockEEProm[addr] << 8);
//    	if(addr < 10)
//    	{
//    		printf("z1:0x%hx\n",result);
//        }

    	result += (uint8_t)(mockEEProm[addr +1]);
//    	if(addr < 10)
//    	{
//    		printf("z2:0x%hx\n",result);
//        }

    	return result;
	}

    void write(int, uint8_t){};
	void writeInt(int addr, int value)
	{
	    //printf("%d, %d, %d, %d\n",addr, value, (int8_t)(value >> 8) & 0xFF, (int8_t)value & 0xFF);
	    mockEEProm[addr] = (int8_t)(value >> 8) & 0xFF;
	    mockEEProm[addr+1] = (int8_t)value & 0xFF;
	}
};


#endif

