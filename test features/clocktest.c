#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// Quick Debug Macros


int main(void) {
    printf("=== STARTING LIVE TIMER TEST ===\n");
    printf("Press any key to stop the timer...\n\n");

    clock_t start = clock();
    
    // This loop runs as fast as the CPU can handle
    while (!kbhit()) { 
        clock_t current = clock();
        double elapsed = (double)(current - start) / CLOCKS_PER_SEC;

        // \r returns the cursor to the start of the line
        // We use space padding at the end to clear old digits
        printf("Time Elapsed: %.2f seconds", elapsed);
        printf("\r"); 
        
        // Optional: slow down the loop slightly so it doesn't 
        // flicker or hog 100% CPU.
        // _sleep(10); 
    }

    // Now that kbhit() detected a key, we actually "catch" the key
    getch(); 
    
    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n\n=== TIMER STOPPED ===\n");
    printf("Final Time: %.4f seconds\n", total_time);

    return 0;
}
