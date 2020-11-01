#include <iostream>
#include "CRC.h"

const uint64_t CRC::m_polynomial = 0x42F0E1EBA9EA3693ull;

CRC::CRC()
{}

CRC::~CRC()
{}

void CRC::create_crc_table()
{}

uint64_t CRC::get_crc_code(uint8_t *stream, int length)
{}

