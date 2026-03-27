#include <stdio.h>
#include <unistd.h>
// Quick Debug Macros

void printnatenchars(void);

int main(void) {

    printf("=== STARTING ISOLATED TEST ===\n\n");
    
    printnatenchars();
    
    printf("\n\n=== TEST COMPLETE ===\n");
    system("pause");
    return 0;
}

void printnatenchars(void) {
    int row = 3;
    int columns = 10;
    printf("    ");
    for (int i = 0; i < columns; i++) printf("%2d ", i);
    printf("\n");
    printf("    ");
    for (int i = 0; i < columns; i++) printf("%2s ", "_");
    printf("\n");
    printf("%2d |         ", row); row++;
    for (int i = 33; i < 255; i++)
    {
        if (i % columns == 0) {printf("\n"); printf("%2d |", row); row++;}
        printf("%2c ", i);
    }
}