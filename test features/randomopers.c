#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUESTIONS 10
#define min_VALUE 1
#define max_VALUE 20

typedef struct {
    int x;
    int y;
    char operation;
    int user_answer;
    int correct_answer;
} MathProblem;

int rng(int min, int max);
char rog();
void getValidXY(int *x, char operation, int *y);
int findCorrect(int x, char operation, int y);
void checkWinner(int answer, int correct);
MathProblem generateProblem(char choice);
int enterInteger(void);

int score = 0;

int main() {
    srand((time(NULL)));

    for (int i = 0; i < QUESTIONS; i++)
    {
        // One line to generate the entire problem
        MathProblem current = generateProblem('-');

        printf("Question %d: %d %c %d = ?\n", i + 1, current.x, current.operation, current.y);
        printf("Answer: ");
        current.user_answer = enterInteger();
        
        checkWinner(current.user_answer, current.correct_answer);
    }
    printf("\n--- FINAL SCORE: %d / %d ---\n", score, QUESTIONS);

    return 0;
}

int rng(int min,  int max){
    return rand() % (max - min + 1) + min;
}

char rog(){
    switch (rng(1,4)) // Fixed the rng range
    {
        case 1: return '+';
        case 2: return '-';
        case 3: return '*';
        case 4: return '/';
    }
}

void getValidXY(int *x, char operation, int *y)
{
    switch (operation)
    {
        case '+':
        case '-':
            // Both + and - use the same generation rules, so we can stack them!
            *x = rng(min_VALUE, max_VALUE);
            *y = rng(min_VALUE, max_VALUE);
            break;
        case '*':
            *x = rng(min_VALUE, max_VALUE);
            *y = rng(min_VALUE, max_VALUE/2);
            break;
        case '/':
            *y = rng(min_VALUE + 1, max_VALUE); 
            int temp_answer = rng(1, max_VALUE/1.5);
            *x = (*y) * temp_answer; 
            break;
    }
}

int findCorrect(int x, char operation, int y){
    switch (operation)
    {
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
        case '/': return x / y;
        default: return 0;
    }
}

void checkWinner(int answer, int correct){
    if (answer == correct)
    {
        printf("nice tama pre\n\n");
        score++;
    }
    else
    {
        printf("mali pre yung tama %d\n\n", correct);
    }
}

MathProblem generateProblem(char choice) {
    MathProblem p;

    // Logic: If the user chose '?', we roll a random sign.
    // Otherwise, we use the specific sign they picked (e.g., '+')
    if (choice == '?') {
        p.operation = rog(); // Your Random Operator Generator
    } else {
        p.operation = choice;
    }

    // Now get the numbers based on that operation
    getValidXY(&p.x, p.operation, &p.y);
    
    // Calculate the math
    p.correct_answer = findCorrect(p.x, p.operation, p.y);

    return p;
}

int enterInteger(void) {
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