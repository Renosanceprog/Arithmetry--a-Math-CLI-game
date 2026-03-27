#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUESTIONS 4
#define min_VALUE 1
#define max_VALUE 20

int rng(int min, int max);
int getrandQ(char *questionstring);
int getAddQ(char *questionstring);
int getSubQ(char *questionstring);
int getMulQ(char *questionstring);
int getDivQ(char *questionstring);
void checkWinner(int answer, int correct);

int score = 0;

int main() {
    srand((time(NULL)));

    for (int i = 0; i < QUESTIONS; i++)
    {
        char question[25];
        int answer;
        int correct;
        correct = getrandQ(question);
        printf("%s\nAnswer: ", question);
        scanf("%d", &answer);
        checkWinner(answer, correct);
    }
    printf("you got a score of %d", score);

    return 0;
}

int rng(int min,  int max){
    return rand() % (max - min + 1) + min;
}

void checkWinner(int answer, int correct){
    if (answer == correct)
    {
        printf("nice tama pre\n");
        score++;
    }
    else
    {
        printf("mali pre yung tama %d\n", correct);
    }
}

int getrandQ(char *questionstring)
{
    switch (rng(1,4))
    {
    case 1: return getAddQ(questionstring);
        break;
    case 2: return getSubQ(questionstring);
        break;
    case 3: return getMulQ(questionstring);
        break;
    case 4: return getDivQ(questionstring);
        break;
    }
}

int getAddQ(char *questionstring)
{
    int x,y;
    x = rng(min_VALUE, max_VALUE);
    y = rng(min_VALUE, max_VALUE);
    sprintf(questionstring, "%d + %d = ?", x, y);
    return x+y;
}

int getSubQ(char *questionstring)
{
    int x,y;
    x = rng(min_VALUE, max_VALUE);
    y = rng(min_VALUE, max_VALUE);
    sprintf(questionstring, "%d - %d = ?", x, y);
    return x-y;
}

int getMulQ(char *questionstring)
{
    int x,y;
    x = rng(min_VALUE, max_VALUE);
    y = rng(min_VALUE, max_VALUE/2);
    sprintf(questionstring, "%d * %d = ?", x, y);
    return x*y;
}

int getDivQ(char *questionstring)
{
    int x,y;
    x = rng(min_VALUE+1, max_VALUE*5);
    do
    {
        y = rng(min_VALUE+1, max_VALUE);
    } while (!((x % y) == 0));
    sprintf(questionstring, "%d / %d = ?", x, y);
    return x/y;
}
