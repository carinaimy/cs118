#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include "CRC.h"

#define INITCRC (0)
const unsigned long long m_polynomial = 0x42F0E1EBA9EA3693;
unsigned long long m_CRC_table[256];
void create_crc_table() {
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

unsigned long long get_crc_code(unsigned char *stream, int length)
{
    int i;
    unsigned long long crc = INITCRC;
    
	i = 0;
	while (i < length)
	{
		crc = m_CRC_table[(crc ^ *stream++) & 0xff] ^ (crc >> 8);
		i++;
	}
 
    return crc;
}