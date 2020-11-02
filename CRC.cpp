#include <iostream>
#include "CRC.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

const uint64_t CRC::m_polynomial = 0x42F0E1EBA9EA3693ull;
#define INITCRC (0)

CRC::CRC()
{}

CRC::~CRC()
{}

void CRC::create_crc_table()
{
    int i, j;
	unsigned long long part;
	for (i = 0; i < 256; i++)
	{
	    part = i;
	    for (j = 0; j < 8; j++)
	    {
		if (part & 1)
		    part = (part >> 1) ^ m_polynomial;
		else
		    part >>= 1;
	    }
	    m_CRC_table[i] = part;
	}
}

uint64_t CRC::get_crc_code(uint8_t *stream, int length)
{
    unsigned long long crc = INITCRC;
    
	int i = 0;
	while (i < length)
	{
		crc = m_CRC_table[(crc ^ *stream++) & 0xff] ^ (crc >> 8);
		i++;
	}
 
    return crc;
}



