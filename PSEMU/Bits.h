#pragma once

#define ANY_BIT_SET(value, bit) (((value) & (bit)) != 0)
#define ALL_BITS_SET(value, bit) (((value) & (bit)) == bit)
