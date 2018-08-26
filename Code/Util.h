#include "miosix.h"
#include <cstdio>

#define BAUD 9600;
#define CPU_clk 100000000;
#define Mantissa 0x28B;



// Some macros for easy Bit manipulation
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))



