//v1 of program
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_USERLEN 50
#define MAX_PASSLEN 20
#define QUESTIONS 4
#define min_VALUE 1
#define max_VALUE 20

#define C_RESET  "\033[0m"
#define C_GREEN  "\033[1;32m"
#define C_YELLOW "\033[1;33m"
#define C_CYAN   "\033[1;36m"
#define C_RED    "\033[1;31m"

typedef struct {
    char choice;
    int lives;
    int qs;
    int points_gained;
    int score;
    int streak;
    double time_spent;
} GameState;

typedef struct {
    int x,y,z;
    char operation, operation2;
    int user_answer;
    int correct_answer;
    int is_boss;
} MathProblem;

typedef struct {
    int Classic;
    int Addition;
    int Subtraction;
    int Multiplication;
    int Division;
    int Corrects;
    int Wrongs;
    int Streak;
    double fastest_answer;
}Scores;


typedef struct Node {
    int id;
    char userNames[MAX_USERLEN];
    char userPasswords[MAX_PASSLEN];
    Scores userScores;
    struct Node *next_user;
}loginInfo;

// game logic functions ==========================
int start_Game();
void ask_Question(GameState *state, MathProblem *curProblem);
void process_Answer(GameState *state, MathProblem *curProblem);
char showGamemodes();
int rng(int min, int max);
char rog();
void getValidXY(int *x, char operation, int *y, int diffRange);
void getValidBossXYZ(int *x, char oper1, int *y,char oper2, int *z, int diffRange);
int findCorrect(int x, char operation, int y);
int findBossCorrect(int x, char op1, int y, char op2, int z);
void checkWinner(int answer, int correct);
void printScoreHype(int pointsGained, double timeTaken, int streak);
int givePoints(int isBoss, char operation, double time, int streak, int qs);
int saveScore(char choice, int score);
void revealScore(int score);
MathProblem generateProblem(char choice, int qs);
MathProblem generateBossProblem(char choice, int qs);
int enterInteger(void);

// login functions ===============================
int start_Login();
int showLoginMenu();
int loginUser();
int registerUser();

void hidePassword(char *password);
void saveData();
void loadData();

// User functions ================================
int start_Hub();
int showMenu();
int showCategoryMenu();
void showUserStats();
int getScore(loginInfo user, int category);
int compareUsers(const void *a, const void *b);
int showLeaderboard(int category);

// Global variables ==============================
int score = 0;
loginInfo *head = NULL; // top of the list
loginInfo *tail = NULL; // bottom of the list
loginInfo *current_User = NULL; // the user who succesfully logged in
int userCount = 0;

// Main Entry ====================================
int main()
{
    srand(time(NULL)); // init rand seed
    loadData();
    if (start_Login() == -1) goto programexit;
    if (start_Hub() == -1) goto programexit;

    programexit:
    saveData();
    return 0;
}

// game logic functions ==========================
int start_Game(){
    GameState state = {showGamemodes(), 3, 1, 0, 0, 0};
    while (state.lives > 0)
    {
        if (state.streak >= 5) printf("%d CORRECT ANSWER STREAK! x%.2f Multiplier!\n", state.streak, (1 + ((state.streak - 4) * 0.1)));
        if (state.qs > 1) printScoreHype(state.points_gained, state.time_spent ,state.streak);
        MathProblem curProblem;
        ask_Question(&state, &curProblem);
        process_Answer(&state, &curProblem);
        printf("It took you %.2lf seconds to answer this question!\n", state.time_spent);
        system("pause"); system("cls");
        int c; while ((c = getchar()) != '\n' && c != EOF);
        state.qs++; // increments to the next round
    }
    system("cls");
    if (saveScore(state.choice, state.score)) printf("NEW PERSONAL BEST!\n");
    revealScore(state.score);
    showLeaderboard(6);
}
void ask_Question(GameState *state, MathProblem *curProblem)
{
        if (state->qs % 10 == 0 && state->qs != 0) {
            *curProblem = generateBossProblem(state->choice, state->qs);
            printf("lives: %d\n!!! BOSS QUESTION !!!\n", state->lives);
            printf("Solve: %d %c %d %c %d = ", curProblem->x, curProblem->operation, curProblem->y, curProblem->operation2, curProblem->z);
        } else {
            *curProblem = generateProblem(state->choice, state->qs);
            printf("lives: %d\nQuestion %d: %d %c %d = ?\n", state->lives, state->qs, curProblem->x, curProblem->operation, curProblem->y);
        }
        printf("Answer: ");
        clock_t start = clock();
        curProblem->user_answer = enterInteger();
        clock_t end = clock();
        state->time_spent = (double)(end - start) / CLOCKS_PER_SEC;
}
void process_Answer(GameState *state, MathProblem *curProblem)
{
    checkWinner(curProblem->user_answer, curProblem->correct_answer);
    if (curProblem->user_answer != curProblem->correct_answer) /*User gets it wrong*/
    {
        state->lives--; 
        current_User->userScores.Wrongs++; 
        state->streak = 0;
    }
    else /*User gets it Right*/
    {
        state->points_gained= givePoints(curProblem->is_boss,curProblem->operation, state->time_spent, state->streak, state->qs); //calculate the points given for that round
        state->score += state->points_gained;
        current_User->userScores.Corrects++; //add stat for corrects
        state->streak++; //increase the streak count
        if (current_User->userScores.fastest_answer == 0.0f || state->time_spent < current_User->userScores.fastest_answer) // checks and updates for fastest time
        {
            current_User->userScores.fastest_answer = state->time_spent; 
            printf("NEW FASTEST ANSWER TIME! ", current_User->userScores.fastest_answer);
        }
    }
    if (state->streak > current_User->userScores.Streak) current_User->userScores.Streak = state->streak; // updates the highest streak achieved by user
}
char showGamemodes()
{
    printf("===== GAMEMODES ======\n");
    printf("[1] Classic\n[2] Addition Only\n[3] Subtraction Only\n[4] Multiplication Only\n[5] Division Only\nChoose option: ");
    while (1)
    {
        switch (enterInteger())
        {
        case 1: return '?';
        case 2: return '+';
        case 3: return '-';
        case 4: return '*';
        case 5: return '/';
        default:
            printf("Choose a valid option.\n");
            break;
        }
    }
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

void getValidXY(int *x, char operation, int *y, int diffRange)
{
    switch (operation)
    {
        case '+':
        case '-':
            // Both + and - use the same generation rules, so we can stack them!
            *x = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
            *y = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
            break;
        case '*':
            *x = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
            *y = rng(min_VALUE+(diffRange/2), (max_VALUE+diffRange)/2);
            break;
        case '/':
            *y = rng(min_VALUE+(diffRange/2) + 1, max_VALUE+diffRange); 
            int temp_answer = rng(1, (max_VALUE+diffRange)/1.5);
            *x = (*y) * temp_answer; 
            break;
    }
}

void getValidBossXYZ(int *x, char op1, int *y, char op2, int *z, int diffRange)
{
    if ((op2 == '*' || op2 == '/') && (op1 == '+' || op1 == '-'))
    {
        getValidXY(y,op2,z,diffRange);
        *x = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
    else if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
    else if ((op1 == '*') && (op2 == '*'))
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), (max_VALUE+diffRange)/4);
    }
    else if ((op1 == '/') && (op2 == '/'))
    {
        *z = rng(2, (max_VALUE + diffRange) / 4 + 2); 
        int temp_answer = rng(1, (max_VALUE + diffRange) / 4 + 2);
        int op1Eval = (*z) * temp_answer; 
        *y = rng(2, (max_VALUE + diffRange) / 4 + 2);
        *x = op1Eval * (*y);
    }
    else
    {
        getValidXY(x,op1,y,diffRange);
        *z = rng(min_VALUE+(diffRange/2), max_VALUE+diffRange);
    }
}

int findBossCorrect(int x, char op1, int y, char op2, int z) 
{
    if ((op2 == '*' || op2 == '/') && (op1 == '+' || op1 == '-')) {
        int right_side = findCorrect(y, op2, z); 
        return findCorrect(x, op1, right_side); 
    } else {
        int left_side = findCorrect(x, op1, y);
        return findCorrect(left_side, op2, z);
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
        printf("Correct!\n\n");
        score++;
    }
    else printf("Nice try! Correct answer is %d\n\n", correct);
}

void printScoreHype(int pointsGained, double timeTaken, int streak) 
{
    printf("\n"); // Give it some breathing room

    // Tier 1: The "God Gamer" (Fast AND High Streak)
    if (timeTaken <= 3.0 && streak >= 5) {
        printf(C_YELLOW "  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF \n");
        printf("  \xB3  GODLIKE COMBO!  \xB3 \n");
        printf("  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9 \n");
        printf("    +" C_RED "%d" C_YELLOW " POINTS!!! \n" C_RESET, pointsGained);
    }
    // Tier 2: The "Speed Demon" (Just fast)
    else if (timeTaken <= 3.0 && pointsGained != 0) {
        printf(C_CYAN "  >>> QUICK DRAW! <<< \n");
        printf("      +%d PTS \n" C_RESET, pointsGained);
    }
    // Tier 3: The "Consistent" (Just high streak)
    else if (streak >= 5 && pointsGained != 0) {
        printf(C_GREEN "  >> STREAK x%d! << \n", streak);
        printf("     +%d PTS \n" C_RESET, pointsGained);
    }
    else if (pointsGained == 0) {
    }
    // Tier 4: The "Standard Correct" (Normal)
    else {
        printf(C_GREEN "      +%d PTS \n" C_RESET, pointsGained);
    }
    printf("\n");
}
int givePoints(int isBoss, char operation, double time, int streak, int qs){
    float points = 20;
    float streakmulti = 1;
    float timeHandicap = ((qs / 10) * 5)/3.5f;
    switch (operation)
    {
        case '+': points *= 1; break;
        case '-': points *= 1.2f; break;
        case '*': points *= 1.5f; break;
        case '/': points *= 2; break;
    }
    if (time < 3+timeHandicap) points *= 1.5f;
    else if (time < 10+timeHandicap) points *= 1;
    else points *= 0.45f;
    if (streak >= 5) {
    int bonus_score = streak - 4; 
    streakmulti = 1.0f + (bonus_score * 0.1f); 
    }
    points *= streakmulti;
    if(isBoss) points *= 10;
    return (int)(points + 0.5f);
}

int saveScore(char choice, int score)
{
    int *scorepointer;
    switch (choice)
    {
        case '?': scorepointer = &current_User->userScores.Classic; break;
        case '+': scorepointer = &current_User->userScores.Addition; break;
        case '-': scorepointer = &current_User->userScores.Subtraction; break;
        case '*': scorepointer = &current_User->userScores.Multiplication; break;
        case '/': scorepointer = &current_User->userScores.Division; break;
    }
    if (score > *scorepointer) {*scorepointer = score; return 1;}
    else return 0;
}

void revealScore(int score)
{
    int step;
    if (score > 50) step = score / 100;
    else step = score;
    for (int i = 0; i < score; i++)
    {
            printf("\r  [TOTAL SCORE]: " C_YELLOW "%03d" C_RESET, i);
            fflush(stdout);
            Sleep(20);
        i+=step;
    }
    printf("\r  [TOTAL SCORE]: " C_YELLOW "%03d" C_RESET "\n", score);
}

MathProblem generateProblem(char choice, int qs)
{
    MathProblem p;
    p.is_boss = 0;
    if (choice == '?') p.operation = rog();
    else p.operation = choice;
    int diffRange = (qs / 10) * 5;
    getValidXY(&p.x, p.operation, &p.y, diffRange);
    p.correct_answer = findCorrect(p.x, p.operation, p.y);
    return p;
}

MathProblem generateBossProblem(char choice, int qs)
{
MathProblem p;
    p.is_boss = 1; // Flag this as a boss question
    // 1. Roll the operators
    if (choice == '?'){p.operation = rog(); p.operation2 = rog();}
    else{p.operation = choice;p.operation2 = choice;}
    int diffRange = (qs / 20) * 5;
    // 2. Roll the numbers (You can reuse your getValidXY logic here, 
    // or just write a quick randomizer for boss numbers)
    getValidBossXYZ(&p.x, p.operation, &p.y, p.operation2, &p.z, diffRange);

    // 3. Calculate the final answer
    p.correct_answer = findBossCorrect(p.x, p.operation, p.y, p.operation2, p.z);
    
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

// login functions ===============================
int start_Login()
{
    do
    {
        printf("===== REGISTER / LOGIN =====\n");
        switch (showLoginMenu())
        {
        case 1: registerUser(); break;
        case 2: loginUser(); break;
        case 3: return -1;
        default:
            printf("Choose a valid option\n");
            break;
        }
        
    } while (current_User == NULL);
    system("pause");
    system("cls");
    return 0;
}

int showLoginMenu()
{
    printf("Current Users = %d\n[1] Register\n[2] Login\n[3] Exit\nChoose Option: ", userCount);
    int choice = enterInteger();
    return choice;
}

int loginUser()
{
    if (userCount == 0)
    {
        return -1;
    }
    
    int valid = 0;
    int index = 0;
    loginInfo *walker;
    // username
    char tempName[MAX_USERLEN];
    do
    {
        if (valid != 0) printf("\33[1A\33[2K\rUsername not Found! ");
        printf("Enter username: ");
        scanf(" %49[^\n]", tempName);
        walker = head;
        while (walker != NULL)
        {
            valid = strcmp(walker->userNames, tempName);
            if (valid == 0) break;
            walker = walker->next_user;
        }
    } while (valid != 0);
    char tempPass[MAX_PASSLEN];
    do
    {
        if (valid != 0) printf("\33[1A\33[2K\rIncorrect Password! ");
        printf("Enter Password: ");
        hidePassword(tempPass);
            valid = strcmp(walker->userPasswords, tempPass);
            if (valid == 0) break;
    } while (valid != 0);
    current_User = walker;
    printf("%s succesfully logged in!\n", current_User->userNames);
    return 0;
}

int registerUser() 
{
    loginInfo *new_user = (loginInfo *)calloc(1, sizeof(loginInfo));
    if (new_user == NULL) return -1; // Memory check!

    // IMPORTANT: Clear the memory! 
    // This sets next_user to NULL and all scores to 0 in one go.
    memset(new_user, 0, sizeof(loginInfo)); 

    userCount++;
    new_user->id = userCount;

    printf("Enter username: ");
    scanf(" %49[^\n]", new_user->userNames);

    char password1[MAX_PASSLEN], password2[MAX_PASSLEN];
    printf("Enter password: ");
    hidePassword(password1);
    
    // Simple Loop for confirmation
    int valid = 1;
    do {
        printf("Confirm password: ");
        hidePassword(password2);
        valid = strcmp(password1, password2);
        if (valid != 0) printf("Mismatch! Try again.\n");
    } while (valid != 0);

    strcpy(new_user->userPasswords, password1);

    // Linked List Connection
    if (head == NULL) {
        head = new_user;
        tail = new_user;
    } else {
        tail->next_user = new_user;
        tail = new_user;
    }
    tail->next_user = NULL; // Extra safety

    current_User = new_user;
    printf("%s successfully Registered!\n", new_user->userNames);
    return 0;
}

void hidePassword(char *password)
{
    int i = 0;
    char c;
    // 1. Changed to an infinite loop! 
    // The ONLY way out is pressing Enter.
    while (1) 
    {
        c = getch();
        
        if (c == '\r' || c == '\n')
        {
            if (i <= 4) continue; // Enforce minimum length (5 chars)
            break; // Submit the password
        }
        else if (c == 8 || c == 127)
        {
            if (i > 0){printf("\b \b"); i--;}
        }
        else if (c == 32) continue;
        else 
        {
            // 2. The Brick Wall: Only accept the character if there is room!
            // If the array is full, we simply ignore the keystroke.
            if (i < MAX_PASSLEN - 1) 
            {
                password[i] = c;
                printf("*");
                i++;
            }
        }
    }
    
    password[i] = '\0'; 
    printf("\n");
}

void saveData()
{
    // We open the file regardless. If the list is empty, 
    // the file becomes empty (which is correct).
    FILE *pData = fopen("programdata.txt", "w");
    if (pData == NULL) { 
        printf("Error opening file\n"); 
        return;
    }

    loginInfo *walker = head;
    while (walker != NULL) {
        fprintf(pData, "%d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%lf\n", 
                walker->id, 
                walker->userNames, 
                walker->userPasswords,
                walker->userScores.Classic,
                walker->userScores.Addition,
                walker->userScores.Subtraction,
                walker->userScores.Multiplication,
                walker->userScores.Division,
                walker->userScores.Corrects,
                walker->userScores.Wrongs,
                walker->userScores.Streak,
                walker->userScores.fastest_answer);
        walker = walker->next_user;
        tail->next_user = NULL;
    }
    fclose(pData);
    printf("Data saved successfully.\n");
}

void loadData()
{
    userCount = 0;
    // Temporary variables to hold the scores during reading
    int id, s1, s2, s3, s4, s5, c, w, strk;
    double fsttm;
    char username[MAX_USERLEN];
    char password[MAX_PASSLEN];
    
    FILE *pData = fopen("programdata.txt", "r");
    if(pData == NULL){ 
        printf("No existing database found. Starting fresh.\n"); 
        return;
    }

    // The format string now expects 8 items (3 strings/id + 5 scores)
    // We use %[^,] for the password too, just in case.
    while (fscanf(pData, "%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%lf\n", &id, username, password, &s1, &s2, &s3, &s4, &s5, &c, &w, &strk, &fsttm) == 12)
    {
        loginInfo *loaded_user = (loginInfo *)malloc(sizeof(loginInfo));
        userCount++;
        loaded_user->id = id;
        strcpy(loaded_user->userNames, username);
        strcpy(loaded_user->userPasswords, password);
        
        // Assigning the loaded scores to the nested struct
        loaded_user->userScores.Classic = s1;
        loaded_user->userScores.Addition = s2;
        loaded_user->userScores.Subtraction = s3;
        loaded_user->userScores.Multiplication = s4;
        loaded_user->userScores.Division = s5;
        loaded_user->userScores.Corrects = c;
        loaded_user->userScores.Wrongs = w;
        loaded_user->userScores.Streak = strk;
        loaded_user->userScores.fastest_answer = fsttm;
        if (head == NULL) {
            head = loaded_user;
            tail = loaded_user;
        }
        else
        {
            tail->next_user = loaded_user;
            tail = loaded_user;
        }
        
    }
    fclose(pData);
}
// User functions ================================
int start_Hub()
{
    while (1)
    {
        printf("===== GAME HUB =====\n");
        switch (showMenu())
        {
        case 1: start_Game(); break;
        case 2: system("cls"); showUserStats(); break;
        case 3: system("cls"); if (showLeaderboard(showCategoryMenu()) == -1) goto avoidpause; break;
        case 4: return -1;
        default: printf("Choose Valid Option.\n"); break;
        }
        system("pause");
        avoidpause:
        system("cls");
    }
    return 0;
}

int showMenu()
{
    printf("[1] Play\n[2] Show Stats\n[3] Show leaderboard\n[4] Exit\nChoose Option: ");
    int choice = enterInteger();
    return choice;
}

int showCategoryMenu()
{
    printf("[1] Classic\n[2] Addition Only\n[3] Subtraction Only\n[4] Multiplication Only\n[5] Division Only\n[6] Overall\n[7] Return to Hub\nChoose Option: ");
    int choice = enterInteger();
    system("cls");
    return choice;
}

void showUserStats()
{
    printf("==== USER STATS ====\n");
    printf("ID: %02d\n",current_User->id);
    printf("Username: %s\n\n",current_User->userNames);
    if (current_User->userScores.Wrongs != 0) printf("Corrects: %2d\nWrongs: %4d\nC:W Ratio: %.3f\n",current_User->userScores.Corrects, current_User->userScores.Wrongs, (current_User->userScores.Corrects / (float)current_User->userScores.Wrongs));
    printf("Highest Streak: %d questions\n",current_User->userScores.Streak);
    if (current_User->userScores.fastest_answer != 0) printf("Fastest time took to answer question: %.2lf seconds\n\n",current_User->userScores.fastest_answer);
    int overallScore = current_User->userScores.Classic + current_User->userScores.Addition + current_User->userScores.Subtraction + current_User->userScores.Multiplication + current_User->userScores.Division;
    printf("    -=(Top Scores)=-");
    printf("\n%-16s | %5d","Classic",current_User->userScores.Classic);
    printf("\n%-16s | %5d","Addition",current_User->userScores.Addition);
    printf("\n%-16s | %5d","Subtraction",current_User->userScores.Subtraction);
    printf("\n%-16s | %5d","Multiplication",current_User->userScores.Multiplication);
    printf("\n%-16s | %5d","Division",current_User->userScores.Division);
    printf("\n%-16s | %5d\n","Overall",overallScore);
    return;
}

int getScore(loginInfo user, int category) {
    switch(category) {
        case 1: return user.userScores.Classic;
        case 2: return user.userScores.Addition;
        case 3: return user.userScores.Subtraction;
        case 4: return user.userScores.Multiplication;
        case 5: return user.userScores.Division;
        case 6: return user.userScores.Classic + user.userScores.Addition + 
                       user.userScores.Subtraction + user.userScores.Multiplication + 
                       user.userScores.Division;
        default: return 0;
    }
}

int current_sort_category = 0;

int compareUsers(const void *a, const void *b) {
    // 1. Cast the 'void' pointers back to 'loginInfo' pointers
    // Since we are sorting an array of POINTERS, 'a' is a pointer to a pointer.
    loginInfo *userA = *(loginInfo **)a;
    loginInfo *userB = *(loginInfo **)b;

    int scoreA = getScore(*userA, current_sort_category);
    int scoreB = getScore(*userB, current_sort_category);

    // Return >0 if B should come before A (Descending order)
    return (scoreB - scoreA); 
}

int showLeaderboard(int category) {
    if (category == 7) return -1;
    if (head == NULL) printf("No users Found.\n");
    else
    {
        // 1. Prepare the pointer array
        loginInfo *rankings[userCount];
        loginInfo *curr = head;
        for (int i = 0; i < userCount; i++) {
            rankings[i] = curr;
            curr = curr->next_user;
        }
    
        // 2. set the sort
        current_sort_category = category;
        qsort(rankings, userCount, sizeof(loginInfo *), compareUsers);
    
        // 3. Print the formatted table
        char *titles[] = {"", "CLASSIC", "ADDITION", "SUBTRACTION", "MULTIPLICATION", "DIVISION", "OVERALL"};
        
        printf("\n--- %s LEADERBOARD ---\n", titles[category]);
        printf("%-4s %-20s %-10s\n", "RANK", "USERNAME", "SCORE");
        printf("------------------------------------------\n");
    
        int rank = 1;
        for (int i = 0; i < 10; i++) {
            if (i > userCount-1) break;
            int score = getScore(*(rankings[i]), category);
            if (score == 0) continue;
            printf("%-4d %-20s %-10d\n", rank, rankings[i]->userNames, score);
            rank++;
        }
    }
    printf("\n");
    return 0;
}

