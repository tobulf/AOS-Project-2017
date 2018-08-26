#include "LED.h"
#include <cstdio>
#include "miosix.h"


using namespace std;
using namespace miosix;

typedef Gpio<GPIOA_BASE, 5> LED1;



LED::LED() {	
	LED1::mode(Mode::OUTPUT);
	toogle = true;
}



void LED::BLINKONCE(int ms) {
	LED1::high();
	Thread::sleep(ms);
	LED1::low();
	Thread::sleep(ms);
};




void LED::BLINK(int ms) {
	while (toogle){
		LED1::high();
		Thread::sleep(ms);
		LED1::low();
		Thread::sleep(ms);
	};

}



void LED::DEBUGBLINK() {
	for (int n = 0; n < 15; n++) {
		LED1::high();
		Thread::sleep(100);
		LED1::low();
		Thread::sleep(100);
	}

}


