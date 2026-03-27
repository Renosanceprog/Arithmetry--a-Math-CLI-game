#include <stdio.h>
#include <conio.h>
#include <windows.h> // Just for the Sleep() function

int main() {
    int running = 1;
    int counter = 0;

    printf("Press 'q' to quit, or any other key to see it registered.\n");

    while (running) {
        // 1. Check if a key was pressed (Non-blocking)
        if (_kbhit()) {
            // 2. Read the key
            char key = _getch(); 
            
            if (key == 'q') {
                running = 0;
                printf("\nExiting loop...\n");
            } else {
                printf("\nYou pressed: %c\n", key);
            }
        }

        // 3. The program continues doing other work uninterrupted
        printf("\rRunning game logic... Frame: %d", counter++);
        Sleep(10); // Pause for 100ms so the console doesn't blur
    }

    return 0;
}