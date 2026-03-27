#include <stdio.h>
#include <windows.h> // Required for Beep() and Sleep()

// Musical note frequencies in Hertz (Hz)
#define G4 392
#define A4 440
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784

// Note durations in milliseconds (ms)
#define EIGHTH 250
#define QUARTER 500
#define HALF 1000

// Helper function to play a note and add a slight pause
void playNote(int frequency, int duration) {
    Beep(frequency, duration);
    // A brief 50ms pause is necessary between notes. 
    // Otherwise, two consecutive notes of the same pitch (like "Hap-py") 
    // will blend into one continuous sound.
    Sleep(50); 
}

int main() {
    printf("Playing Happy Birthday...\n");

    // Line 1: Happy Birthday to you
    playNote(G4, EIGHTH);
    playNote(G4, EIGHTH);
    playNote(A4, QUARTER);
    playNote(G4, QUARTER);
    playNote(C5, QUARTER);
    playNote(B4, HALF);

    // Line 2: Happy Birthday to you
    playNote(G4, EIGHTH);
    playNote(G4, EIGHTH);
    playNote(A4, QUARTER);
    playNote(G4, QUARTER);
    playNote(D5, QUARTER);
    playNote(C5, HALF);

    // Line 3: Happy Birthday dear user
    playNote(G4, EIGHTH);
    playNote(G4, EIGHTH);
    playNote(G5, QUARTER);
    playNote(E5, QUARTER);
    playNote(C5, QUARTER);
    playNote(B4, QUARTER);
    playNote(A4, HALF);

    // Line 4: Happy Birthday to you
    playNote(F5, EIGHTH);
    playNote(F5, EIGHTH);
    playNote(E5, QUARTER);
    playNote(C5, QUARTER);
    playNote(D5, QUARTER);
    playNote(C5, HALF);

    printf("Done!\n");
    return 0;
}