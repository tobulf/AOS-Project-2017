#include <miosix.h>
#include <cstdio>
#include "Util.h"
#include "Button.h"


using namespace std;
using namespace miosix;


Button::Button(){
    BUTTON_BOARD::mode(Mode::INPUT);
    BUTTON_1::mode(Mode::INPUT);
    BUTTON_2::mode(Mode::INPUT);
};

//For the button on the Nucleo board:
bool Button::Board_Pressed(){
    
    if (BUTTON_BOARD::value() == 0){
        return true;
    }
    
    else{
        return false;
    }
    
    
};

// Button 1:
bool Button::One_Pressed(){
    
    if (BUTTON_1::value() > 0){
        return true;
    }
    
    else{
        return false;
    }
    
    
};

//Button 2:
bool Button::Two_Pressed(){
    
    if (BUTTON_2::value() > 0){
        return true;
    }
    
    else{
        return false;
    }
    
    
};


// Poll on buttons untill one is pressed, then return said button:
int Button::PollButtons(){
    bool pressed = false;
    while(!pressed){
        if (BUTTON_BOARD::value() == 0){
            pressed = true;
            return ButtonBoard;
        }
        else if (BUTTON_1::value() > 0){
            pressed = true;
            return Button1;
        } 
        else if (BUTTON_2::value() > 0){
            pressed = true;
            return Button2;
        }  
    }
};