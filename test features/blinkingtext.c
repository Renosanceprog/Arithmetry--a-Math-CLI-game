#include <stdio.h>
#include <conio.h>
#include <windows.h> // Required for the Sleep() function

int main() {
    printf("Logging in...\n");

    // Loop until the user presses a key
    while (!kbhit()) {
        // 1. Print the red text
        printf("\r\033[31m[ ACCESS DENIED ]\033[0m");
        Sleep(500); // Wait 500 milliseconds (half a second)

        // 2. Erase the text using spaces, but stay on the same line
        printf("\r                 \r");
        Sleep(500); // Wait another half second
    }

    // Clear the buffer of whatever key they pressed to stop the blink
    getch(); 
    
    printf("\nMoving on to the next screen...\n");
    return 0;
}