#include <cstdio>

#define BAUD 9600;
#define CPU_clk 100000000;
#define Mantissa 0x28B;



// Some macros for easy Bit manipulation
#define set_bit(reg, bit ) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit ) (reg & (1 << bit))



//Enums used for game states:
enum GameState {Setup, Playing, GameOver};
enum ButtonName {Button1 = 1, Button2, ButtonBoard};
