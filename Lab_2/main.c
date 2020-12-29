// Rojan Karn - ADC Conversion

//////////////////////////////////////////////////////////////////
// THIS LAB DEMONSTRATES ANALOG TO DIGITAL CONVERSION USING ADC //
// - Takes analog input from a potentiometer and converts it to //
//   a digital signal that is used to light up 1 of 8 LEDs.     //
//////////////////////////////////////////////////////////////////

// I/O:
// Potentiometer (POT)
// Two Pushbuttons (PB1, PB2)
// Two Lines of LEDs: LEDA[0-7] and LEDB[0-7]

// Objective: Convert a hexadecimal number to binary and enter into circuitry

// This program is a game which gives the user a random hexadecimal number and the
// user is tasked with converting that number into binary in less than 10 seconds.
// Input is through a line of LEDs which act as bits. Every three rounds, the given
// number will become larger, increasing the difficulty of the game.

// Timer: Timer 0 in 16-bit SYSCLK/12


#define RIN 661887480 // for simulator use

#include "C8051_SIM.h" // simulator header file
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void Port_Init(void);      // Initialize ports for input and output
void Timer_Init(void);     // Initialize Timer 0
void Interrupt_Init(void); //Initialize interrupts
uint8_t read_AD_input(uint8_t pin_number);
unsigned char random(char roundNumber);

// Inputs
#define PB1     P1_5
#define PB2     P1_2
#define POT     P1_6

// Outputs
#define LEDA0   P2_0
#define LEDA1   P2_1
#define LEDA2   P2_2
#define LEDA3   P2_3
#define LEDA4   P2_4
#define LEDA5   P2_5
#define LEDA6   P2_6
#define LEDA7   P2_7

#define LEDB0   P3_0
#define LEDB1   P3_1
#define LEDB2   P3_2
#define LEDB3   P3_3
#define LEDB4   P3_4
#define LEDB5   P3_5
#define LEDB6   P3_6
#define LEDB7   P3_7

unsigned int counts;
uint8_t input;

void main(void) {
    // initialize all systems
    Sys_Init();
    Port_Init();
    Timer_Init();
    Interrupt_Init();
    ADC_Init();

    // instructions
    printf("Instructions: Given a Hex number, use the LEDs to represent that number in binary\r\n");
    printf("Press any key to start program \r\n\n");
    input = getchar();

    // main loop
    while(1) {
        Sim_Update();
        printf("Press and Release PB2 to start game\r\n");
        while (PB2) Sim_Update(); // wait for user to press button
        Game_Loop(); // main body of code in separate function
    }
}

void Game_Loop(void) {
    // initialize variables needed
    unsigned char random_seeder;
    unsigned char LED_value;
    unsigned int player_answer;
    signed int player_score = 0;
    bool not_submitted = true; // flag for submission

    // wait for PB2 to be released
    while(!PB2) Sim_Update();

    counts = 0;
    for(unsigned int i = 1; i < 10; i++) {
        Sim_Update();

        bool not_submitted = true;
        // get number to convert and print it
        random_seeder = random(i);
        printf("Convert: 0x%02X\t\t", random_seeder);

        // clear player answer and LEDB
        player_answer = 0;
        LEDB0 = LEDB1 = LEDB2 = LEDB3 = LEDB4 = LEDB5 = LEDB6 = LEDB7 = 1;

        // start round timer
        TR0 = 1;

        // loop until time runs out for this round (10 seconds)
        while (counts < 282) {
            Sim_Update();
            // read ADC value
            input = read_AD_input(6);

            // method to light up proper LEDA
            LED_value = input/32;  // equals 8 to distribute to all 8 LEDs

            // bits are flipped: LED_value 7 = LEDA0, LED_value 6 = LEDA1
            if (LED_value == 7) {
                LEDA0 = 0; // = 0 means on, = 1 means off
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 1;
                // if PB1 is pushed, turn on the corresponding LEDB
                if (!PB1) {
                    LEDB0 = 0;
                }
            } else if (LED_value == 6) {
                LEDA0 = 1;
                LEDA1 = 0;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB1 = 0;
                }
            } else if (LED_value == 5) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 0;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB2 = 0;
                }
            } else if (LED_value == 4) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 0;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB3 = 0;
                }
            } else if (LED_value == 3) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 0;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB4 = 0;
                }
            } else if (LED_value == 2) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 0;
                LEDA6 = 1;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB5 = 0;
                }
            } else if (LED_value == 1) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 0;
                LEDA7 = 1;
                if (!PB1) {
                    LEDB6 = 0;
                }
            } else if (LED_value == 0) {
                LEDA0 = 1;
                LEDA1 = 1;
                LEDA2 = 1;
                LEDA3 = 1;
                LEDA4 = 1;
                LEDA5 = 1;
                LEDA6 = 1;
                LEDA7 = 0;
                if (!PB1) {
                    LEDB7 = 0;
                }
            }

            // checking if player wants to reset answer
            if (!PB1) {
                if (!PB2) {
                    LEDB0 = LEDB1 = LEDB2 = LEDB3 = LEDB4 = LEDB5 = LEDB6 = LEDB7 = 1; // turn all LEDs off
                    while (!PB2) Sim_Update(); // wait for player to release button
                }
            }

            // check for submission
            if (!PB2 && PB1) {
                // update submission flag
                not_submitted = false;

                // corresponding values to each LEDB
                if (!LEDB0) {
                    player_answer += 1;  // updating the player's answer (bit 0: value 1)
                }

                if (!LEDB1) {
                    player_answer += 2; // bit 1: value 2
                }

                if (!LEDB2) {
                    player_answer += 4; // bit 2: value 4
                }

                if (!LEDB3) {
                    player_answer += 8; // bit 3: value 8
                }

                if (!LEDB4) {
                    player_answer += 16; // bit 4: value 16
                }

                if (!LEDB5) {
                    player_answer += 32; // bit 5: value 32
                }

                if (!LEDB6) {
                    player_answer += 64; // bit 6: value 64
                }

                if (!LEDB7) {
                    player_answer += 128; // bit 7: value 128
                }

                // checking if the player was right or wrong
                if (player_answer == random_seeder) {
                    printf("Correct!\t\t");
                    // updating the score based on the amount of whole seconds left
                    player_score += (282 - counts) / 28.2;
                } else {
                    printf("Wrong...\t\t");
                    // penalizing for wrong answer
                    player_score -= (282 - counts) / 28.2;
                }

                // print score
                printf("Score: %d\r\n", player_score);
                // wait for PB2 to be released
                while (!PB2) Sim_Update();
                // forcing the loop to end by pushing the timer to the end
                counts = 281;
            }
        }
        counts = 0; // reset counter

        // timeout
        if (not_submitted) {
            printf("Timeout!\t\t");
            player_score -= 1;
            // print score
            printf("Score: %d\r\n", player_score);
        }
    }

    // printing final score
    printf("\nGame Over, Final Score: %d\r\n", player_score);

}



void Port_Init(void) {
    // Port 1
    P1MDIN &= ~0x40;
    P1MDOUT &= ~0x64;
    P1 |= 0x64;

    // Port 2
    P2MDOUT |= 0xFF;
    P2 = 0xFF;

    // Port 3
    P3MDOUT |= 0xFF;
    P3 = 0xFF;
}

// Initialize Timer 0
void Timer_Init(void) {
    CKCON &= 0xF7;
    TMOD &= 0x0D;
    TMOD |= 0x01;
    TR0 = TL0 = TH0 = 0;
}

// Initialize the Interrupt for Timer 0
void Interrupt_Init(void) {
    ET0 = 1;
    ET1 = 0;
    IE |= 0x82;
}

// Initialize the ADC (Analog to Digital Converter)
void ADC_Init(void) {
    REF0CN = 0x03;
    ADC1CN = 0x80;
    ADC1CF &= ~0x03;
    ADC1CF |= 0x03;
}

// Function to convert the analog value to digital using ADC
uint8_t read_AD_input(uint8_t pin_number)
{
    AMX1SL = pin_number; // channel 2
    ADC1CN &= ~0x20; // clear "conversion completed" flag
    ADC1CN |= 0x10; // initiate A/D conversion

    while ((ADC1CN & 0x20) == 0x00) Sim_Update(); // wait for conversion to complete

	return ADC1;  // return the converted value
}

// Returns a random number 0-3
unsigned char random(char roundNumber) {
    // return a random number based on which round it is (how many bits it should be)
    if (roundNumber < 4) {
        return (rand()%16);
    } else if (roundNumber < 7) {
        return (rand()%64);
    } else if (roundNumber < 10) {
        return (rand()%256);
    }
}

// Interrupt Service Routine
void Timer_0_ISR(void) {
    counts++; // simply increment counter global variable
}

// By: Rojan Karn
