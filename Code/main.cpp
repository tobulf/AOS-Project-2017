#include <cstdio>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <miosix.h>
#include "ADC-driver.h"
#include "Button.h"
#include "util.h"
#include "miosix/util/lcd44780.h"
#include <stdlib.h>


using namespace std;
using namespace miosix;

//Def Buzzer Pin-Out:
typedef Gpio<GPIOC_BASE,0> BUZZOUT;
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

// Initialize Objects:
Button Buttons;
Adc Potentiometer;

// Mutex for data protection:
pthread_mutex_t adc_mtx  = PTHREAD_MUTEX_INITIALIZER;


// Shared variables:
int potentiometer_value = 0;
int random_note = 0;

// Buzzer control variables:
bool player_note = false;
bool mute = true;


// Buzzer function:
void buzz(long frequency, long length) {
    // Delay between notes:
	long delayValue = 1000000 / frequency / 2;
    // calculate the number of cycles:
	long numCycles = frequency * length / 1000;
	int intdelay = ((int) (delayValue));
	for (long i = 0; i < numCycles; i++) {
        // write the buzzer pin high:
		BUZZOUT::high();
        // wait for the calculated delay value:
		usleep(intdelay);
        // write the buzzer pin low:
		BUZZOUT::low();
        // wait for the calculated delay value:
		usleep(intdelay);
	}
}






// Adc thread, constantly updates the value of the pot meter:
void *adcthread(void*){
    uint16_t temp_val = 0;
    for(;;){
        temp_val = Potentiometer.read();
        pthread_mutex_lock(&adc_mtx);
        if (temp_val != potentiometer_value){
            //Mutex for data protection.
            potentiometer_value = temp_val;
        }
        pthread_mutex_unlock(&adc_mtx);
    }
    return NULL;
}
// Buzzer Thread:
void *buzzerthread(void*){
    int note = 0;
    for(;;){
        // When a random note is played:
        if(!player_note && !mute){
            note = random_note*10;
            buzz(note, 100);
        }
        // When player chooses what note to play:
        else if(player_note && !mute){
            pthread_mutex_lock(&adc_mtx);
            if(note != potentiometer_value){
                note = potentiometer_value*10;
            }
            pthread_mutex_unlock(&adc_mtx);
            buzz(note, 100);
        }
    }
    return NULL;
}





void *gamethread(void*){
    // Set Initial state:
    GameState state = Setup;
    // Variable for loop-control:
    bool EndGame = false;
    // Initialize random var:
    int rnd;
    // initialize N players, default: 2.
    int NPlayers = 2;
    // Variables to save the players guess:
    int Guess1 = 0;
    int Guess2 = 0;
    // Variables to remember the right answer:
    int PlayedNote1 = 0;
    int PlayedNote2 = 0;
    // Startup text:
    display.printf("Hello welcome to buzzer");
    Thread::sleep(2000);
    display.printf("The game where");
    Thread::sleep(3000);
    display.printf("you guess notes.");
    Thread::sleep(3000);
    //Statemachine:

    while(!EndGame){
        switch (state){
            // Setup state, really here for further development, 3P etc...
            case Setup:
                display.printf("Press 2 to start:");
                // Wait for user to press button
                while(!Buttons.Two_Pressed()); 
                // Start game:
                state = Playing;
                break;
            // Playing state:
            case Playing:
                // Get a random number to play a random tune:
                display.printf("Player1 Ready:");
                Thread::sleep(5000);
                for (int n = 0; n != NPlayers; n++){
                    rnd = rand() % 255;
                    display.printf("Playing Tune:");
                    random_note = rnd;
                    mute = false;
                    Thread::sleep(5000);
                    display.printf("Press 1 to Choose:");
                    player_note = true;
                    // Wait for player to confirm choice:
                    while(!Buttons.One_Pressed());
                    if (n==0){
                        pthread_mutex_lock(&adc_mtx);
                        Guess1 = potentiometer_value;
                        pthread_mutex_unlock(&adc_mtx);
                        // Save the right answer:
                        PlayedNote1 = rnd;
                        // Mute and reset buzzer control
                        player_note = false;
                        mute = true;
                        display.printf("Player2 Ready:");
                        Thread::sleep(5000);
                    }
                    else if (n==1){
                        pthread_mutex_lock(&adc_mtx);
                        Guess2 = potentiometer_value;
                        pthread_mutex_unlock(&adc_mtx);
                        // Save the right answer:
                        PlayedNote2 = rnd;
                        player_note = false;
                        mute = true;
                    }
                }
                state = GameOver;
                break;
        
            case GameOver:
                // Find which guess was closest:
                Guess1 = abs(Guess1 - PlayedNote1);
                Guess2 = abs(Guess2 - PlayedNote2);
                // Check with eachother:
                if(Guess1 == Guess2){
                    display.printf("TIE!");
                    Thread::sleep(5000);
                }
                else if(Guess1 > Guess2){
                    display.printf("Player 2 wins!");
                    Thread::sleep(5000);

                }
                else if(Guess1 < Guess2){
                    display.printf("Player 1 wins!");
                    Thread::sleep(5000);
                }
                // Keep on playing:
                state = Setup;

                break;
    
            default:
                break;
           
        }

    }
   return NULL;
};



int main(){
    // Declare threads:
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    // Spawn threads:
    pthread_create(&t1, NULL, &adcthread, NULL);
    pthread_create(&t2, NULL, &buzzerthread, NULL);
    pthread_create(&t3, NULL, &gamethread, NULL);
    // Join:
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);


};
