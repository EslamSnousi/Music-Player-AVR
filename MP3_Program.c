#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "../../HAL/icu/ICU_Interface.h"
#include "Stack_Interface.h"

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

// Music notes of the song, 0 is a rest/pulse
u16 notes[] = {

NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
NOTE_A4, NOTE_G4, NOTE_A4, 0,

NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
NOTE_A4, NOTE_G4, NOTE_A4, 0,

NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
NOTE_D5, NOTE_E5, NOTE_A4, 0,
NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
NOTE_C5, NOTE_A4, NOTE_B4, 0,

NOTE_A4, NOTE_A4,
//Repeat of first part
		NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
		NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
		NOTE_A4, NOTE_G4, NOTE_A4, 0,

		NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
		NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
		NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
		NOTE_A4, NOTE_G4, NOTE_A4, 0,

		NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
		NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
		NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
		NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

		NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
		NOTE_D5, NOTE_E5, NOTE_A4, 0,
		NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
		NOTE_C5, NOTE_A4, NOTE_B4, 0,
		//End of Repeat

		NOTE_E5, 0, 0, NOTE_F5, 0, 0,
		NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
		NOTE_D5, 0, 0, NOTE_C5, 0, 0,
		NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

		NOTE_E5, 0, 0, NOTE_F5, 0, 0,
		NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
		NOTE_D5, 0, 0, NOTE_C5, 0, 0,
		NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4 };

// Durations (in ms)

u8 durations[] = { 125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125,
		250, 125, 125, 125, 125, 375, 125,

		125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125, 250, 125,
		125, 125, 125, 375, 125,

		125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125, 250, 125,
		125, 125, 125, 125, 250, 125,

		125, 125, 250, 125, 125, 250, 125, 250, 125, 125, 125, 250, 125, 125,
		125, 125, 375, 375,

		250, 125,
		//Rpeat of First Part
		125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125, 375, 125,

		125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125, 250, 125,
		125, 125, 125, 375, 125,

		125, 125, 250, 125, 125, 125, 125, 250, 125, 125, 125, 125, 250, 125,
		125, 125, 125, 125, 250, 125,

		125, 125, 250, 125, 125, 250, 125, 250, 125, 125, 125, 250, 125, 125,
		125, 125, 375, 375,
		//End of Repeat

		250, 125, 375, 250, 125, 375, 125, 125, 125, 125, 125, 125, 125, 125,
		375, 250, 125, 375, 250, 125, 375, 125, 125, 125, 125, 125, 500,

		250, 125, 375, 250, 125, 375, 125, 125, 125, 125, 125, 125, 125, 125,
		375, 250, 125, 375, 250, 125, 375, 125, 125, 125, 125, 125, 500 };

volatile u16 counter = 0;

void playTone(unsigned int frequency, unsigned int duration)
{
    unsigned int i;
    unsigned long delay = 16000000 / frequency / 2;  // Calculate the delay value

    unsigned long num_cycles = frequency * duration / 1000; // Calculate the number of cycles

    for (i = 0; i < num_cycles; i++)
    {
        PORTD |= (1 << PD5);    // Set PD5 high to turn on the buzzer
        _delay_us(delay);       // Delay for half the period
        PORTD &= ~(1 << PD5);   // Set PD5 low to turn off the buzzer
        _delay_us(delay);       // Delay for half the period
    }
}
int main() {
	M_Timer_Void_PWMInit(TIMER1_CHANNEL);
	M_Timer_Void_PWMSetDutyCycle(TIMER1_CHANNEL, 50);
	M_Timer_Void_PWMStart(TIMER1_CHANNEL);
	H_Buzzer_Void_BuzzerInit();

	while (1) {
		playTone(notes[counter], durations[counter]);
		counter++;
        delay_ms(1000);
	}
	return 0;
}
