
#include <cstdio>
#include "miosix.h"
#include "ADC-driver.h"
#include "util.h"
#include "miosix/util/lcd44780.h"

using namespace std;
using namespace miosix;

//Typedefs:
//Button definition
typedef Gpio<GPIOC_BASE,13> BUTTON;
//Definition Input Display:
typedef Gpio<GPIOB_BASE,13> D4;
typedef Gpio<GPIOB_BASE,14> D5;
typedef Gpio<GPIOB_BASE,15> D6;
typedef Gpio<GPIOB_BASE,1>  D7;
typedef Gpio<GPIOC_BASE,4>  RS;
typedef Gpio<GPIOA_BASE,10>  E;

//Display object, part of Miosix:
Lcd44780 display(RS::getPin(),E::getPin(),D4::getPin()
        ,D5::getPin(),D6::getPin(),D7::getPin(),2,16);





//Initialize Drivers:
//LED Led;



int main(){
    Adc potentiometer;
    for(;;){
        uint16_t i = potentiometer.read();
        Thread::sleep(10);
        display.clear();
        display.printf("%d",i);
        
        //const char* fisk = "halla balla";
        //Print((const char*) i);
        i++;
    };
};
