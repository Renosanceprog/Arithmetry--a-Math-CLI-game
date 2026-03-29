#include <stdio.h>
#include <unistd.h>
#include <windows.h>

// Quick Debug Macros
int validinput(void);

int main(void) {
    SetConsoleTitle("C Sandbox - Isolated Test Environment");
    while (1)
    {
        printf("=== STARTING ISOLATED TEST ===\n\n");
        printf("Choose Sound:\n");
        printf("1. HYPE sound\n");
        printf("2. Game over\n");
        printf("3. Rising chime\n");
        printf("4. falling chime\n");
        printf("5. Boss Alarm\n");
        printf("6. High Score\n");
        printf("7. Menu Cursor Move\n");
        printf("8. Menu Select\n");
        printf("9. Game Start\n");
        printf("10. Error / Blocked Input\n");
        printf("11. Boss Defeated\n");
        printf("12. 1-Up / Extra Life\n");
        //printf("13. HYPE\n");
        switch (validinput())
        {
        case 1:
            Beep(1046, 100); // C6 note
            Beep(1318, 150); // E6 note
            break;
        case 2:
                Beep(250, 150);
                Beep(150, 250);
            break;
        case 3:
            Beep(523, 50);  // C5
            Beep(659, 50);  // E5
            Beep(784, 50);  // G5
            Beep(1046, 150); // C6
            break;
        case 4:
            Beep(622, 150); 
            Beep(587, 150);
            Beep(554, 150);
            Beep(523, 400);
            break;
        case 5:
// [SFX: Boss Alarm] - Heavy alternating siren
Beep(300, 350); 
Beep(150, 300); 
//Sleep(50);
Beep(300, 350); 
Beep(150, 300);
            break;
        case 6:
// [SFX: High Score] - Triumphant fanfare
Beep(523, 100); // C5
Beep(523, 100); // C5
Beep(523, 100); // C5
Beep(415, 150); // Ab4
Beep(466, 150); // Bb4
Beep(523, 300); // C5 (Sustained victory note)
            break;
        case 7:
// [SFX: Menu Cursor Move] - Extremely short, hollow blip
Beep(800, 15);
            break;
        case 8:
// [SFX: Menu Select] - Snappy double-chirp
Beep(1046, 30); // C6
Beep(1567, 50); // G6
            break;
        case 9:
// [SFX: Error / Blocked Input] - Short, low thud
Beep(150, 60);
Beep(100, 80);
            break;
        case 10:
// [SFX: Game Start] - Quick ascending major triad
Beep(523, 100);  // C5
Beep(784, 100);  // G5
Beep(1046, 250); // C6 (Held)
            break;
        case 11:
// [SFX: Boss Defeated] - Epic mini-fanfare
Beep(1046, 100); // C6
Beep(784, 100);  // G5
Beep(523, 100);  // C5
Beep(784, 100);  // G5
Beep(1046, 100); // C6
Beep(1567, 400); // G6 (Sustained victory!)
            break;
        case 12:
// [SFX: 1-Up / Extra Life] - Rapid, sparkling climb
Beep(1318, 70); // E6
Beep(1567, 70); // G6
Beep(2093, 70); // C7
Beep(2637, 150); // E7 (Very high, sparkly peak)
            break;
        case 13:
// MIDI Sequence: 240 BPM
            // Note Lengths: 1/16th note = ~63ms
            
            Beep(294, 375); // D4 (6/16)
            Beep(349, 125); // F4 (2/16)
            Beep(392, 250); // G4 (4/16)
            
            Sleep(63);      // Rest (1/16)
            
            Beep(440, 375); // A4 (6/16)
            
            Sleep(63);      // Rest (1/16)
            
            Beep(349, 125); // F4 (2/16)
            Beep(330, 250); // E4 (4/16)
            
            Sleep(63);      // Rest (1/16)
            
            Beep(294, 375); // D4 (6/16) - The final resolving note
            break;
            
        default:
            break;
        }
        system("cls");
    }
    printf("\n\n=== TEST COMPLETE ===\n");
    system("pause");
    return 0;
}

int validinput(void) {
    int valid = 0, input;
    do {
        if (scanf(" %d", &input) != 1) {
            printf("Invalid input. Enter value: ");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        valid = 1;
    } while (!valid);
    return input;
}