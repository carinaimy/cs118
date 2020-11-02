#include <stdint.h>

#ifndef CRC_H
#define CRC_H

void create_crc_table();
unsigned long long get_crc_code(unsigned char *stream, int length);


#endif
