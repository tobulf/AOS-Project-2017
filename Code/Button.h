#include <miosix.h>



/*
BUTTON CLASS
*/


class Button{
    // Defining which pins that are going to be buttons:
    typedef miosix::Gpio<GPIOC_BASE,13> BUTTON_BOARD;
    typedef miosix::Gpio<GPIOA_BASE,1> BUTTON_1;
    typedef miosix::Gpio<GPIOA_BASE,0> BUTTON_2;
private:
    /* data */
public:
    Button();
    int PollButtons();
    bool Board_Pressed();
    bool One_Pressed();
    bool Two_Pressed();
};
