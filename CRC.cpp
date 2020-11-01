#include <iostream>
#include "CRC.h"

#define INITCRC (0)
const uint64_t CRC::m_polynomial = 0x42F0E1EBA9EA3693ull;
unsigned long long CRCTable[256];

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
	    CRCTable[i] = part;
	}
}

uint64_t CRC::get_crc_code(uint8_t *stream, int length)
{
    int i, low, high;
    unsigned long long crc = INITCRC;
    
	i = 0;
	while (i < length)
	{
		crc = CRCTable[(crc ^ *stream++) & 0xff] ^ (crc >> 8);
		i++;
	}
 
    return crc;
}

