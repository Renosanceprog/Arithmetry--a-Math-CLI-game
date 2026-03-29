//v1 of program
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <stdint.h> 

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
#define C_BLUE   "\033[1;34m"
#define C_MAG    "\033[1;35m"
#define C_INVERT "\033[1;30m\033[47m"

// Input Engine Macros
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESC 27

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

typedef enum {
    // UI & Menus
    SFX_MENU_MOVE,
    SFX_MENU_KEY_TYPING,
    SFX_MENU_SUCCESS,
    SFX_MENU_INVALID,
    SFX_MENU_BLOCKED,
    SFX_MENU_ESCAPE,
    SFX_MENU_CONTINUE,

    // Game Loop
    SFX_START,
    SFX_NORMAL_CORRECT,
    SFX_QUICK_CORRECT,
    SFX_GODLIKE_CORRECT,
    SFX_STREAK_BURST,
    SFX_WRONG,
    SFX_BOSS_ALARM,
    SFX_BOSS_DEFEAT,
    SFX_GAMEOVER,
    SFX_SCORE_TICK,
    SFX_SCORE_FINAL,
    SFX_HIGHSCORE
} SoundID;

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

int getVisibleInput(char *buffer, int max_len);
int hidePassword(char *password);
void saveData();
void loadData();

void clearInnerRow(int row);
// User functions ================================
int start_Hub();
int showMenu();
int showCategoryMenu();
void showUserStats();
int getScore(loginInfo user, int category);
int compareUsers(const void *a, const void *b);
int showLeaderboard(int category);
// UI Functions ==================================
int getKeyPress();
void printMenu(int start_row, char* title, char** options, int num_options, int cursor_pos); 
int runInteractiveMenu(int start_row, char* title, char** options, int num_options);
void initWindow();
void drawBorder(const char* color);
void printCentered(int row, char* text, char* color);
void drawGameLogos(int screen_id);
const char* getBorderColor(int streak, int step);
void bootIntro();
// Sound Functions ===============================
DWORD WINAPI SoundThread(LPVOID lpParam);
void playAudio(SoundID id);
// Global variables ==============================
loginInfo *head = NULL; // top of the list
loginInfo *tail = NULL; // bottom of the list
loginInfo *current_User = NULL; // the user who succesfully logged in
int userCount = 0;

// Main Entry ====================================
int main()
{
    SetConsoleTitle("Arithmetry! - Test your Arithmetic Skills! | Version 1.2");
    srand(time(NULL)); // init rand seed
    loadData();
    initWindow();
    drawBorder(C_RESET);
    bootIntro();
    if (start_Login() == -1) goto programexit;
    if (start_Hub() == -1) goto programexit;

    programexit:
    saveData();
    return 0;
}

// game logic functions ==========================
int start_Game(){
    GameState state = {showGamemodes(), 3, 1, 0, 0, 0};
    if (state.choice == -1) return 0; 
    playAudio(SFX_START); 
    Sleep(300);
    while (state.lives > 0)
    {
        MathProblem curProblem;
        system("cls");
        drawBorder(getBorderColor(state.streak, 1));

        char header[100];
        sprintf(header, "LIVES: %d   |   SCORE: %d   |   STREAK: %d", state.lives, state.score, state.streak);
        printCentered(3, header, C_CYAN);

        ask_Question(&state, &curProblem);
        process_Answer(&state, &curProblem);

        char timeMsg[50];
        sprintf(timeMsg, "It took you %.2lf seconds to answer!", state.time_spent);
        printCentered(20, timeMsg, C_RESET);

        if (state.streak >= 5) {
            playAudio(SFX_STREAK_BURST);
            char streakMsg[50];
            sprintf(streakMsg, "%d CORRECT ANSWER STREAK! x%.2f Multiplier!", state.streak, (1 + ((state.streak - 4) * 0.1)));
            printCentered(22, streakMsg, C_YELLOW);
            Sleep(200);
        }

        printf("\033[23;1H");
        printScoreHype(state.points_gained, state.time_spent, state.streak);

        printCentered(28, "[ Press any key to continue ]", "\033[0;37m");
        getch(); 
        if (state.lives > 0) playAudio(SFX_MENU_CONTINUE);
        state.qs++; 
    }
    playAudio(SFX_GAMEOVER);
    system("cls");
    for (int i = 0; i < 3; i++)
    {
        system("color 47");
        Sleep(106);
        system("color 07");
        Sleep(106);
        system("color 47");
        Sleep(53);
        system("color c7");
        Sleep(53);
    }
    system("color 07");
    system("cls");
    drawBorder(C_YELLOW);
    drawGameLogos(6); 
    if (saveScore(state.choice, state.score)) {
        playAudio(SFX_HIGHSCORE);
        printCentered(12, "NEW PERSONAL BEST!", C_GREEN);
    }
    printf("\033[14;1H");
    revealScore(state.score);

    printCentered(28, "[ Press any key to view Leaderboard ]", "\033[0;37m");
    getch();
    playAudio(SFX_MENU_CONTINUE);
    system("cls");
    drawBorder("\e[38;5;207m");
    showLeaderboard(6);
    getch();
    playAudio(SFX_MENU_CONTINUE);
    return 0;
}

void ask_Question(GameState *state, MathProblem *curProblem)
{
    char qBuffer[100];
    int promptCol = 35; 

    if (state->qs % 10 == 0 && state->qs != 0) {
        *curProblem = generateBossProblem(state->choice, state->qs);
        sprintf(qBuffer, "Solve: %d %c %d %c %d = ?", curProblem->x, curProblem->operation, curProblem->y, curProblem->operation2, curProblem->z);
        playAudio(SFX_BOSS_ALARM);
        system("color 4");
        drawBorder("\e[38;5;196m");
        Sleep(350); 
        system("color 47");
        Sleep(50); 
        printCentered(6, "!!! BOSS QUESTION !!!", C_RESET);
        system("color 47"); 
        Sleep(250);
        printCentered(8, qBuffer, C_RESET);
        system("color 4");
        Sleep(350);
        printf("\033[10;%dH" "\e[38;5;196m" "Answer: " C_RESET, promptCol); // Turns prompt red
        Sleep(300);
    } 
    else 
    {
        *curProblem = generateProblem(state->choice, state->qs);
        sprintf(qBuffer, "Question %d:  %d %c %d = ?", state->qs, curProblem->x, curProblem->operation, curProblem->y);
        
        printCentered(8, qBuffer, C_YELLOW);
        printf("\033[10;%dH" C_CYAN "Answer: " C_RESET, promptCol);
    }

    clock_t start = clock();
    curProblem->user_answer = enterInteger();
    clock_t end = clock();
    
    state->time_spent = (double)(end - start) / CLOCKS_PER_SEC;
}

void process_Answer(GameState *state, MathProblem *curProblem)
{
    printf("\033[14;25H"); 
    checkWinner(curProblem->user_answer, curProblem->correct_answer); 

    if (curProblem->user_answer != curProblem->correct_answer) 
    {
        playAudio(SFX_WRONG);
        drawBorder("\033[38;2;12;12;12m");
        Sleep(150);
        drawBorder("\e[38;5;196m");
        state->lives--; 
        current_User->userScores.Wrongs++; 
        state->streak = 0;
        state->points_gained = 0;
    }
    else 
    {
        if (curProblem->is_boss == 1) playAudio(SFX_BOSS_DEFEAT);
        state->points_gained = givePoints(curProblem->is_boss, curProblem->operation, state->time_spent, state->streak, state->qs); 
        state->score += state->points_gained;
        current_User->userScores.Corrects++; 
        state->streak++; 
        
        if (current_User->userScores.fastest_answer == 0.0f || state->time_spent < current_User->userScores.fastest_answer) 
        {
            playAudio(SFX_HIGHSCORE);
            current_User->userScores.fastest_answer = state->time_spent;    
            printCentered(18, "NEW FASTEST ANSWER TIME!", C_GREEN);
        }
    }
    
    if (state->streak > current_User->userScores.Streak) {
        current_User->userScores.Streak = state->streak; 
    }
}
char showGamemodes()
{
    system("cls"); drawBorder(C_BLUE); drawGameLogos(5); // Use the cool Blue logo!
    
    char* options[] = {"Classic", "Addition Only", "Subtraction Only", "Multiplication Only", "Division Only", "Return to Hub"};
    
    // Engine handles all arrow keys and Enter/Esc
    int choice = runInteractiveMenu(11, "=== SELECT GAMEMODE ===", options, 6);
    switch (choice)
    {
        case 1: return '?';
        case 2: return '+';
        case 3: return '-';
        case 4: return '*';
        case 5: return '/';
        case 6:  return -1; // They highlighted "Return to Hub"
        case -1: return -1; // They pressed the ESC key
    }
    return -1; 
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
        printCentered(14, "Correct!", C_GREEN);
    }
    else 
    {
        char buffer[50];
        sprintf(buffer, "Nice try! Correct answer is %d", correct);
        printCentered(14, buffer, C_RED);
    }
}

void printScoreHype(int pointsGained, double timeTaken, int streak) 
{
    // Clear the hype area (Rows 23-26) to prevent text ghosting from previous rounds
    clearInnerRow(23); clearInnerRow(24); clearInnerRow(25); clearInnerRow(26);

    if (pointsGained == 0) return; // Do nothing if they got it wrong

    char buffer[100];

    // Tier 1: The "God Gamer" (Fast AND High Streak)
    if (timeTaken <= 3.0 && streak >= 5) {
        playAudio(SFX_GODLIKE_CORRECT);
        printCentered(23, "  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF ", C_YELLOW);
        printCentered(24, "  \xB3  GODLIKE COMBO!  \xB3 ", C_YELLOW);
        printCentered(25, "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9 ", C_YELLOW);
        sprintf(buffer, "+%d POINTS!!!", pointsGained);
        printCentered(26, buffer, C_RED);
    }
    // Tier 2: The "Speed Demon" (Just fast)
    else if (timeTaken <= 3.0) {
        playAudio(SFX_QUICK_CORRECT);
        printCentered(24, ">>> QUICK DRAW! <<<", C_CYAN);
        sprintf(buffer, "+%d PTS", pointsGained);
        printCentered(25, buffer, C_RESET);
    }
    // Tier 3: The "Consistent" (Just high streak)
    else if (streak >= 5) {
        sprintf(buffer, ">> STREAK x%d! <<", streak);
        printCentered(24, buffer, C_GREEN);
        sprintf(buffer, "+%d PTS", pointsGained);
        printCentered(25, buffer, C_RESET);
    }
    // Tier 4: The "Standard Correct" (Normal)
    else {
        playAudio(SFX_NORMAL_CORRECT);
        sprintf(buffer, "+%d PTS", pointsGained);
        printCentered(24, buffer, C_GREEN);
    }
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
    char buffer[50];
    
    if (score > 50) step = score / 100;
    else step = score;
    
    // SAFETY CATCH: Prevent step from ever being 0!
    if (step <= 0) step = 1; 

    // The animation loop
    for (int i = 0; i < score; i += step)
    {
        playAudio(SFX_SCORE_TICK);
        sprintf(buffer, "[ TOTAL SCORE: %03d ]", i);
        printCentered(16, buffer, C_YELLOW);
        Sleep(20);
    }
    
    // Final print to guarantee the exact score lands on the screen
    playAudio(SFX_SCORE_FINAL);
    sprintf(buffer, "[ TOTAL SCORE: %03d ]", score);
    printCentered(16, buffer, C_YELLOW);
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
char buffer[20];
    int i = 0;
    char c;
    
    while (1) {
        c = getch();
        
        if (c == '\r' || c == '\n') {
            if (i == 0) {playAudio(SFX_MENU_INVALID); continue;} // Block Enter if empty!
            if (i == 1 && buffer[0] == '-') {playAudio(SFX_MENU_INVALID); continue;} // Block Enter if just a minus sign!
            break; 
        }
        else if (c == 8 || c == 127) { // Backspace
            if (i > 0) {
                playAudio(SFX_MENU_KEY_TYPING);
                printf("\b \b");
                i--;
            }
            else playAudio(SFX_MENU_INVALID);
        }
        else if (i < 15) {
            // ONLY accept digits, OR a minus sign if it's the very first character
            if ((c >= '0' && c <= '9') || (c == '-' && i == 0)) {
                playAudio(SFX_MENU_KEY_TYPING);
                buffer[i] = c;
                printf("%c", c);
                i++;
            }
            else playAudio(SFX_MENU_INVALID);
        }
    }
    buffer[i] = '\0';
    return atoi(buffer); // Convert the perfect string into an integer
}

// login functions ===============================
int showLoginMenu()
{
    char* options[] = {"Register", "Login", "Exit"};
    
    // We use sprintf to dynamically inject the userCount into the menu title!
    char title[50];
    sprintf(title, "=== REGISTER / LOGIN (Users: %d) ===", userCount);
    
    // Start the menu at row 12, perfectly centered under the logo
    return runInteractiveMenu(12, title, options, 3);
}

int start_Login()
{
    do
    {
        system("cls");
        drawBorder(C_RESET);
        drawGameLogos(1); // Draw the main logo at the top
        
        switch (showLoginMenu())
        {
            case 1: 
                registerUser(); 
                break;
            case 2: 
                loginUser(); 
                break;
            case 3: 
            case -1: // If they press 'Exit' or the ESC key
                return -1;
        }
        
    } while (current_User == NULL);
    
    // We successfully logged in! Clear the screen to transition to the Hub cleanly.
    system("cls");
    drawBorder(C_RESET);
    return 0;
}

int loginUser()
{
    if (userCount == 0) return -1;
    
    // 1. Clean UI Slate
    system("cls"); drawBorder(C_RESET); drawGameLogos(1);
    printCentered(12, "=== LOGIN ===", C_YELLOW);
    printCentered(28, "[ Press ESC to cancel ]", "\033[0;37m");
    
    int valid = 0;
    loginInfo *walker;
    char tempName[MAX_USERLEN];
    do
    {
        clearInnerRow(14);
        clearInnerRow(15);
        if (valid != 0) {playAudio(SFX_MENU_BLOCKED); printCentered(14, "Username not Found!", C_RED);}
        
        printf("\033[15;25H" C_YELLOW "Enter username: " C_RESET);
        
        // Use our new input reader!
        if (getVisibleInput(tempName, MAX_USERLEN) == -1) return -1; 
        
        walker = head;
        while (walker != NULL)
        {
            valid = strcmp(walker->userNames, tempName);
            if (valid == 0) break;
            walker = walker->next_user;
        }
    } while (valid != 0);
    playAudio(SFX_MENU_CONTINUE);
    char tempPass[MAX_PASSLEN];
    do
    {
        clearInnerRow(16);
        clearInnerRow(17);
        if (valid != 0) {playAudio(SFX_MENU_BLOCKED); printCentered(16, "Incorrect Password!", C_RED);}
        
        printf("\033[17;25H" C_YELLOW "Enter Password: " C_RESET);
        
        // Listen for ESC in the password field too!
        if (hidePassword(tempPass) == -1) return -1; 
        
        valid = strcmp(walker->userPasswords, tempPass);
        if (valid == 0) break;
    } while (valid != 0);

    current_User = walker;
    
    // Wipe the input fields to make way for the success message
    clearInnerRow(14); clearInnerRow(15); clearInnerRow(16); clearInnerRow(17); clearInnerRow(28);
    
    char buffer[100];
    sprintf(buffer, "%s successfully logged in!", current_User->userNames);
    playAudio(SFX_MENU_SUCCESS);
    printCentered(16, buffer, C_GREEN);
    Sleep(500); // Pause for 1 second so they can actually read it
    
    return 0;
}

int registerUser() 
{
    system("cls"); drawBorder(C_RESET); drawGameLogos(1);
    printCentered(12, "=== REGISTER ===", C_YELLOW);
    printCentered(28, "[ Press ESC to cancel ]", "\033[0;37m");

    // calloc already zeroes out the memory, so you don't actually need memset here!
    loginInfo *new_user = (loginInfo *)calloc(1, sizeof(loginInfo));
    if (new_user == NULL) return -1; 

    // --- 1. USERNAME INPUT & DUPLICATE CHECK ---
    int is_taken;
    while (1) {
        clearInnerRow(14); // Clear the input row in case we are looping
        
        printf("\033[14;25H" C_YELLOW "Enter username: " C_RESET);
        
        // If they hit ESC, free the memory so we don't leak it, then back out
        if (getVisibleInput(new_user->userNames, 50) == -1) {
            free(new_user); 
            return -1;
        }

        is_taken = 0;
        loginInfo *walker = head;
        
        // Scan the linked list for a match
        while (walker != NULL) {
            if (strcmp(walker->userNames, new_user->userNames) == 0) {
                is_taken = 1;
                break;
            }
            walker = walker->next_user;
        }

        // If taken, play the buzzer and loop back to the top!
        if (is_taken) {
            playAudio(SFX_MENU_BLOCKED); 
            printCentered(18, "Username already taken! Try another.", C_RED);
        } else {
            clearInnerRow(18); // Wipe the error text cleanly
            break; // Unique username found! Break out of the loop.
        }
    }
    playAudio(SFX_MENU_CONTINUE);
    // --- 2. PASSWORD INPUT ---
    char password1[MAX_PASSLEN], password2[MAX_PASSLEN];
    printf("\033[15;25H" C_YELLOW "Enter password: " C_RESET);
    if (hidePassword(password1) == -1) {
        free(new_user);
        return -1;
    }
    
    // --- 3. PASSWORD CONFIRMATION ---
    int valid = 1;
    do {
        clearInnerRow(16);
        clearInnerRow(18); // Clear the error message row
        
        printf("\033[16;25H" C_YELLOW "Confirm password: " C_RESET);
        if (hidePassword(password2) == -1) {
            free(new_user);
            return -1;
        }
        
        valid = strcmp(password1, password2);
        if (valid != 0) {
            playAudio(SFX_MENU_INVALID); // Using our Invalid sound here instead of blocked!
            printCentered(18, "Mismatch! Try again.", C_RED);
        }
    } while (valid != 0);

    // --- 4. SAVE NEW USER ---
    userCount++;
    new_user->id = userCount;
    strcpy(new_user->userPasswords, password1);

    // Append to the Linked List
    if (head == NULL) {
        head = new_user;
        tail = new_user;
    } else {
        tail->next_user = new_user;
        tail = new_user;
    }
    tail->next_user = NULL; 

    current_User = new_user;
    
    // Clean up the UI before showing the success message
    clearInnerRow(14); clearInnerRow(15); clearInnerRow(16); clearInnerRow(28);
    
    char buffer[100];
    sprintf(buffer, "%s successfully Registered!", new_user->userNames);
    playAudio(SFX_MENU_SUCCESS);
    printCentered(16, buffer, C_GREEN);
    Sleep(1000); // Give them a full second to hear the chime and read the text
    
    return 0;
}

int getVisibleInput(char *buffer, int max_len) 
{
    int i = 0;
    char c;
    while (1) {
        c = getch();
        
        if (c == 27){playAudio(SFX_MENU_ESCAPE); return -1;} // Player pressed ESC!
        
        if (c == '\r' || c == '\n') {
            if (i == 0){playAudio(SFX_MENU_INVALID); continue;} // Don't allow empty strings
            break;
        }
        else if (c == 8 || c == 127) { // Backspace
            playAudio(SFX_MENU_KEY_TYPING);
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } 
        else if (i < max_len - 1) {
            playAudio(SFX_MENU_KEY_TYPING);
            buffer[i] = c;
            printf("%c", c);
            i++;
        }
    }
    buffer[i] = '\0';
    return 0; // Success
}

int hidePassword(char *password)
{
    int i = 0;
    char c;
    while (1) 
    {
        c = getch();
        
        if (c == 27){playAudio(SFX_MENU_ESCAPE); return -1;} // Player pressed ESC!
        
        if (c == '\r' || c == '\n')
        {
            if (i <= 4) {playAudio(SFX_MENU_INVALID); continue;}
            break; 
        }
        else if (c == 8 || c == 127)
        {
            if (i > 0){printf("\b \b"); i--;}
            if (i == 0) playAudio(SFX_MENU_KEY_TYPING);
        }
        else if (c == 32) {playAudio(SFX_MENU_INVALID); continue;}
        else 
        {
            if (i < MAX_PASSLEN - 1) 
            {
                playAudio(SFX_MENU_KEY_TYPING);
                password[i] = c;
                printf("*");
                i++;
            }
        }
    }
    
    password[i] = '\0'; 
    return 0; // Success
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

void clearInnerRow(int row) {
    printf("\033[%d;2H", row); // Start at column 2
    for(int i = 0; i < 78; i++) putchar(' '); // Print 78 blank spaces
}
// User functions ================================
int start_Hub()
{
    while (1)
    {
        system("cls");
        drawBorder("\e[38;5;214m");
        drawGameLogos(2); // The Main Hub Logo

        switch (showMenu())
        {
            case 1: start_Game(); break;
            case 2: system("cls"); drawBorder("\e[38;5;121m"); showUserStats(); getch(); playAudio(SFX_MENU_CONTINUE);break;
            case 3: system("cls"); drawBorder("\e[38;5;207m"); drawGameLogos(4);
                int cat = showCategoryMenu();
                if (cat != -1) {
                    system("cls"); drawBorder("\e[38;5;207m"); 
                    showLeaderboard(cat); 
                    // Leaderboard handles its own pause!
                    getch(); 
                    playAudio(SFX_MENU_CONTINUE);
                }
                break;
            case 4: 
            case -1: // If they press ESC on the main menu, exit the game
                return -1; 
        }
    }
    return 0;
}

int showMenu()
{
    // 1. Define the buttons
    char* options[] = {"Play", "Show Stats", "Show Leaderboard", "Exit"};
    
    // 2. Tell the engine to run it at Row 12, Column 30
    return runInteractiveMenu(12, "===== GAME HUB =====", options, 4);
}

int showCategoryMenu()
{
    char* options[] = {"Classic", "Addition Only", "Subtraction Only", "Multiplication Only", "Division Only", "Overall", "Return to Hub"};
    int choice = runInteractiveMenu(11, "=== LEADERBOARD CATEGORY ===", options, 7);
    if (choice == 7) return -1; // Safely escape!
    return choice;
}

void showUserStats()
{
    drawGameLogos(3);
    char buffer[100];
    
    printCentered(10, "==== USER STATS ====", C_YELLOW);
    sprintf(buffer, "Username: %s (ID: %02d)", current_User->userNames, current_User->id);
    printCentered(12, buffer, C_CYAN);
    
    int corrects = current_User->userScores.Corrects;
    int wrongs = current_User->userScores.Wrongs;
    if (wrongs != 0) sprintf(buffer, "Corrects: %d  |  Wrongs: %d  |  C:W Ratio: %.3f", corrects, wrongs, (float)corrects / wrongs);
    else sprintf(buffer, "Corrects: %d  |  Wrongs: 0  |  C:W Ratio: PERFECT", corrects);
    printCentered(14, buffer, C_RESET);
    
    if (current_User->userScores.fastest_answer != 0)sprintf(buffer, "Highest Streak: %d  |  Fastest Time: %.2lf s", current_User->userScores.Streak, current_User->userScores.fastest_answer);
    else sprintf(buffer, "Highest Streak: %d  |  Fastest Time: N/A", current_User->userScores.Streak);

    printCentered(15, buffer, C_RESET);
    printCentered(18, "-=( Top Scores )=-", C_YELLOW);
    
    int col = 28;
    int row = 20;
    
    printf("\033[%d;%dH" C_RESET "%-16s | " C_GREEN "%5d", row++, col, "Classic", current_User->userScores.Classic);
    printf("\033[%d;%dH" C_RESET "%-16s | " C_GREEN "%5d", row++, col, "Addition", current_User->userScores.Addition);
    printf("\033[%d;%dH" C_RESET "%-16s | " C_GREEN "%5d", row++, col, "Subtraction", current_User->userScores.Subtraction);
    printf("\033[%d;%dH" C_RESET "%-16s | " C_GREEN "%5d", row++, col, "Multiplication", current_User->userScores.Multiplication);
    printf("\033[%d;%dH" C_RESET "%-16s | " C_GREEN "%5d", row++, col, "Division", current_User->userScores.Division);
    
    int overallScore = current_User->userScores.Classic + current_User->userScores.Addition + current_User->userScores.Subtraction + current_User->userScores.Multiplication + current_User->userScores.Division;
    printf("\033[%d;%dH" C_CYAN "%-16s | %5d" C_RESET, row + 1, col, "OVERALL", overallScore);
    printCentered(28, "[ Press any key to return ]", "\033[0;37m");
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
    system("cls"); drawBorder("\e[38;5;207m"); drawGameLogos(4);
    if (category == -1) return -1;
    if (head == NULL) {
        printCentered(14, "No users found in database.", "\e[38;5;244m");
        printCentered(28, "[ Press any key to return ]", "\033[0;37m");
        return 0;
    }

    // 1. Prepare the pointer array (Safe memory cap)
    loginInfo *rankings[100];
    loginInfo *curr = head;
    int max_count = userCount > 100 ? 100 : userCount;
    for (int i = 0; i < max_count; i++) {
        rankings[i] = curr;
        curr = curr->next_user;
    }

    // 2. Set the sort
    current_sort_category = category;
    qsort(rankings, max_count, sizeof(loginInfo *), compareUsers);

    // 3. Print the formatted table
    char *titles[] = {"", "CLASSIC", "ADDITION", "SUBTRACTION", "MULTIPLICATION", "DIVISION", "OVERALL"};
    char buffer[100];
    
    sprintf(buffer, "--- %s LEADERBOARD ---", titles[category]);
    printCentered(9, buffer, "\e[38;5;81m"); // Cyan subtitle
    
    // The table is exactly 38 characters wide.
    // X = (80 - 38) / 2 = 21
    int col = 21;
    int row = 11;
    
    printf("\033[%d;%dH" C_YELLOW "%-6s %-20s %-10s" C_RESET, row++, col, "RANK", "USERNAME", "SCORE");
    printf("\033[%d;%dH\e[38;5;244m--------------------------------------" C_RESET, row++, col);

    int rank = 1;
    for (int i = 0; i < 10; i++) {
        if (i > max_count - 1) break;
        int score = getScore(*(rankings[i]), category);
        if (score == 0) continue; 
        
        // Podium Color Logic
        char *color;
        if (rank == 1) color = "\e[38;5;220m";      // Gold
        else if (rank == 2) color = "\e[38;5;250m"; // Silver
        else if (rank == 3) color = "\e[38;5;172m"; // Bronze
        else color = "\e[38;5;244m";                // Dim Grey

        printf("\033[%d;%dH%s%-6d %-20s %-10d" C_RESET, row++, col, color, rank, rankings[i]->userNames, score);
        rank++;
    }

    printCentered(28, "[ Press any key to return ]", "\033[0;37m");
    return 0;
}
// ====== UI FUNCTIONS =====================================================
int getKeyPress() 
{
    int ch = getch();
    
    // Check for the "Warning" byte
    if (ch == 0 || ch == 224) { 
        ch = getch(); // Grab the actual arrow key code
        return ch;
    }
    // If it wasn't an arrow key, just return the normal key (Enter, Esc, etc.)
    return ch; 
}

// Notice we removed 'start_col'
void printMenu(int start_row, char* title, char** options, int num_options, int cursor_pos) 
{
    // Print the title perfectly centered
    printCentered(start_row, title, C_YELLOW);
    
    // 1. Find the length of the longest option in this specific menu
    int max_len = 0;
    for (int i = 0; i < num_options; i++) {
        int len = strlen(options[i]);
        if (len > max_len) max_len = len;
    }

    // 2. Calculate the starting column for the ENTIRE BLOCK
    // We add 8 to account for the "  [ > ] " cursor prefix
    int block_start_col = ((80 - (max_len+16)) / 2);

    // 3. Print all options locked to that single starting column
    for (int i = 0; i < num_options; i++) 
    {
        if (cursor_pos == i + 1) {
            // HIGHLIGHTED: The %-*s tells C to pad the string to max_len spaces
            printf("\033[%d;%dH" C_CYAN "  [ > ] %-*s  " C_RESET, start_row + 2 + i, block_start_col, max_len, options[i]);
        } else {
            // NORMAL: Overwrite with the exact same layout to clear old cursors
            printf("\033[%d;%dH        %-*s  ", start_row + 2 + i, block_start_col, max_len, options[i]); 
        }
    }
}

// Update the engine signature to match
int runInteractiveMenu(int start_row, char* title, char** options, int num_options) 
{
    int cursor_pos = 1; 
    int key;

    while (1) 
    { 
        printMenu(start_row, title, options, num_options, cursor_pos); 
        key = getKeyPress();
        if (key == KEY_UP) {
            playAudio(SFX_MENU_MOVE);
            cursor_pos--;
            if (cursor_pos < 1) cursor_pos = num_options;
        } 
        else if (key == KEY_DOWN) {
            playAudio(SFX_MENU_MOVE);
            cursor_pos++;
            if (cursor_pos > num_options) cursor_pos = 1;
        } 
        else if (key == KEY_ENTER) {playAudio(SFX_MENU_CONTINUE);return cursor_pos;}
        else if (key == KEY_ESC) {playAudio(SFX_MENU_ESCAPE);return -1;}
        else playAudio(SFX_MENU_INVALID);
    }
}

void initWindow()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    
    // Enable Virtual Terminal Processing (ANSI support)
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Lock the window size
    SMALL_RECT windowSize = {0, 0, 79, 29};
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    
    COORD bufferSize = {80, 30};
    SetConsoleScreenBufferSize(hOut, bufferSize);
    
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void drawBorder(const char* color)
{
    printf(color);
    printf("\033[1;1H\xC9");
    for (int i = 0; i < 78; i++) printf("\xCD");
    printf("\xBB");
    for (int i = 2; i < 30; i++)
    {
        printf("\033[%d;1H\xBA\033[%d;80H\xBA", i, i);
    }
    printf("\033[30;1H\xC8");
    for (int i = 0; i < 78; i++) printf("\xCD");
    printf("\xBC");
    printf("\033[2;3H");
}

void printCentered(int row, char* text, char* color)
{
    int len = strlen(text);
    int col = (80 - len) / 2;
    
    // Safety check just in case a string is absurdly long
    if (col < 2) col = 2; 
    
    printf("\033[%d;%dH%s%s" C_RESET, row, col, color, text);
}

// Pass an ID to tell it which screen we are on
void drawGameLogos(int screen_id) 
{
    switch (screen_id) 
    {
        case 1: // TITLE LOGO
        {
            char* logo[] = {
                "    ___         _ __  __                   __             __",
                "   /   |  _____(_) /_/ /_  ____ ___  ___  / /________  __/ /",
                "  / /| | / ___/ / __/ __ \\/ __ `__ \\/ _ \\/ __/ ___/ / / / / ",
                " / ___ |/ /  / / /_/ / / / / / / / /  __/ /_/ /  / /_/ /_/  ",
                "/_/  |_/_/  /_/\\__/_/ /_/_/ /_/ /_/\\___/\\__/_/   \\__, (_)   ",
                "                                                /____/      "
            };
                printCentered(4, logo[0],"\e[38;5;195m");
                printCentered(5, logo[1], "\e[38;5;123m");
                printCentered(6, logo[2], "\e[38;5;45m");
                printCentered(7, logo[3], "\e[38;5;27m");
                printCentered(8, logo[4], "\e[38;5;20m");
                printCentered(9, logo[5], "\e[38;5;18m");
            break;
        }
        
        case 2: // MAIN HUB
        {
            char* logo[] = {
                "     __     __  ___      _ _   __   __  __      ____       __",
                "    / /____/  |/  /___ _(_) | / /  / / / /_  __/ __ )_____/ /",
                "   / /____/ /|_/ / __ `/ /  |/ /  / /_/ / / / / __  /____/ / ",
                "  / /____/ /  / / /_/ / / /|  /  / __  / /_/ / /_/ /____/ /  ",
                " / /    /_/  /_/\\__,_/_/_/ |_/  /_/ /_/\\__,_/_____/    / /   ",
                "/_/                                                   /_/    "
            };
                printCentered(4, logo[0], "\e[38;5;222m"); 
                printCentered(5, logo[1], "\e[38;5;220m"); 
                printCentered(6, logo[2], "\e[38;5;214m"); 
                printCentered(7, logo[3], "\e[38;5;208m"); 
                printCentered(8, logo[4], "\e[38;5;202m"); 
                printCentered(9, logo[5], "\e[38;5;166m");

            break;
        }
        
        case 3: // USER STATS
        {
            char* logo[] = {
                "   __  __                  _____ __        __      ",
                "  / / / /_______  _____   / ___// /_____ _/ /______",
                " / / / / ___/ _ \\/ ___/   \\__ \\/ __/ __ `/ __/ ___/",
                "/ /_/ (__  )  __/ /      ___/ / /_/ /_/ / /_(__  ) ",
                "\\____/____/\\___/_/      /____/\\__/\\__,_/\\__/____/  "
            };
                printCentered(4, logo[0], "\e[38;5;157m");
                printCentered(5, logo[1], "\e[38;5;121m");
                printCentered(6, logo[2], "\e[38;5;82m");
                printCentered(7, logo[3], "\e[38;5;40m");
                printCentered(8, logo[4], "\e[38;5;28m"); 
            break;
        case 4: // LEADERBOARD
        {
            char* logo[] = {
                "     ____                   __          __                         ___ ",
                "   _/_/ /   ___  ____ _____/ /__  _____/ /_  ____  ____ __________/ / |",
                "  / // /   / _ \\/ __ `/ __  / _ \\/ ___/ __ \\/ __ \\/ __ `/ ___/ __  // /",
                " / // /___/  __/ /_/ / /_/ /  __/ /  / /_/ / /_/ / /_/ / /  / /_/ // / ",
                "/ //_____/\\___/\\__,_/\\__,_/\\___/_/  /_.___/\\____/\\__,_/_/   \\__,_//_/  ",
                "|_|                                                             /_/    "
            };
                printCentered(3, logo[0], "\e[38;5;135m");
                printCentered(4, logo[1], "\e[38;5;171m");
                printCentered(5, logo[2], "\e[38;5;207m");
                printCentered(6, logo[3], "\e[38;5;81m");
                printCentered(7, logo[4], "\e[38;5;45m");
                printCentered(8, logo[5], "\e[38;5;45m");
            break;
        }
        case 5: // START GAME
        {
            char* logo[] = {
                "     ________ __             __     ______                     __",
                "    / _/ ___// /_____ ______/ /_   / ____/___ _____ ___  ___  /  /",
                "   / / \\__ \\/ __/ __ `/ ___/ __/  / / __/ __ `/ __ `__ \\/ _ \\ / / ",
                "  / / ___/ / /_/ /_/ / /  / /_   / /_/ / /_/ / / / / / /  __// /  ",
                " / / /____/\\__/\\__,_/_/   \\__/   \\____/\\__,_/_/ /_/ /_/\\___// /   ",
                "/__/                                                      /__/    "
            };
                printCentered(3, logo[0], "\033[38;2;0;150;255m"); 
                printCentered(4, logo[1], "\033[38;2;0;100;200m"); 
                printCentered(5, logo[2], "\033[38;2;0;50;100m");  
                printCentered(6, logo[3], "\033[38;2;5;25;50m");   
                printCentered(7, logo[4], "\033[38;2;15;15;15m");  
                printCentered(8, logo[5], "\033[38;2;12;12;12m");
            break;
        }
        case 6: // GAME OVER / RESULTS
        {
            char* logo[] = {
                "   _________    __  _________    ____ _    _______ ____ ",
                "  / ____/   |  /  |/  / ____/   / __ \\ |  / /__  // __ \\",
                " / / __/ /| | / /|_/ / __/     / / / / | / / /_ </ /_/ /",
                "/ /_/ / ___ |/ /  / / /___    / /_/ /| |/ /___/ / _, _/ ",
                "\\____/_/  |_/_/  /_/_____/   \\____/ |___//____/_/ |_(_)"
            };

            printCentered(4, logo[0], "\033[38;2;255;30;30m"); 
            printCentered(5, logo[1], "\033[38;2;190;0;0m");   
            printCentered(6, logo[2], "\033[38;2;110;0;0m");   
            printCentered(7, logo[3], "\033[38;2;50;10;10m");  
            printCentered(8, logo[4], "\033[38;2;12;12;12m");  
            
            break;
        }
        }
    }
}

#include <math.h> // Add this at the top of your program!

const char* getBorderColor(int streak, int step) {
    // STATIC is required so the string survives after the function returns!
    static char color_buffer[32]; 

    // 2. Default starting color (Reset for streaks 0-4)
    if (streak < 5) return "\033[0m"; // Using absolute reset code for safety

    // 3. The Smooth RGB Math Engine
    // We use sine waves phase-shifted by 120 degrees (2.094 radians) 
    // to create a perfect, infinitely smooth RGB color wheel.
    // Multiplying by 0.3 makes 'step = 1' a very smooth, gradual transition.
    double frequency = 0.3 * step;
    
    int r = (int)(sin(frequency * streak + 0) * 127 + 128);
    int g = (int)(sin(frequency * streak + 2.094) * 127 + 128);
    int b = (int)(sin(frequency * streak + 4.188) * 127 + 128);

    // 4. The "Anti-Red" Filter
    // If the sine wave naturally hits a deep red (High R, Low G, Low B)...
    if (r > 200 && g < 80 && b < 80) {
        g += 70; // ...we inject some Green to shift the hue safely into Orange/Gold!
    }

    // 5. Safely build the True Color RGB ANSI string
    sprintf(color_buffer, "\033[38;2;%d;%d;%dm", r, g, b);
    
    return color_buffer;
}
void bootIntro()
{
            char* logo[] = {
                "    ___         _ __  __                   __             __",
                "   /   |  _____(_) /_/ /_  ____ ___  ___  / /________  __/ /",
                "  / /| | / ___/ / __/ __ \\/ __ `__ \\/ _ \\/ __/ ___/ / / / / ",
                " / ___ |/ /  / / /_/ / / / / / / / /  __/ /_/ /  / /_/ /_/  ",
                "/_/  |_/_/  /_/\\__/_/ /_/_/ /_/ /_/\\___/\\__/_/   \\__, (_)   ",
                "                                                /____/      "
            };
                printCentered(4, logo[0],C_RESET);
                    Beep(294, 375); // D4 (6/16)
                clearInnerRow(4);
                printCentered(5, logo[1], C_RESET);
                    Beep(349, 125); // F4 (2/16)
                clearInnerRow(5);
                printCentered(6, logo[2], C_RESET);
                    Beep(392, 250); // G4 (4/16)
                
                Sleep(63);      // Rest (1/16)
                clearInnerRow(6);
                printCentered(7, logo[3], C_RESET);
                    Beep(440, 375); // A4 (6/16)
                
                Sleep(63);      // Rest (1/16)

                clearInnerRow(7);
                printCentered(8, logo[4], C_RESET);
                    Beep(349, 125); // F4 (2/16)
                clearInnerRow(8);
                printCentered(9, logo[5], C_RESET);
                    Beep(330, 250); // E4 (4/16)
                
                Sleep(63);      // Rest (1/16)
                printCentered(4, logo[0],"\e[38;5;195m");
                Sleep(1);
                printCentered(5, logo[1], "\e[38;5;123m");
                Sleep(1);
                printCentered(6, logo[2], "\e[38;5;45m");
                Sleep(1);
                printCentered(7, logo[3], "\e[38;5;27m");
                Sleep(1);
                printCentered(8, logo[4], "\e[38;5;20m");
                Sleep(1);
                printCentered(9, logo[5], "\e[38;5;18m");
                    Beep(294, 375); // D4 (6/16) - The final resolving note
}
// Sound functions
DWORD WINAPI SoundThread(LPVOID lpParam)
{
    // Safely cast the passed parameter back into our SoundID enum
    SoundID id = (SoundID)(intptr_t)lpParam; 

switch (id) 
    {
        // --- UI & MENUS ---
        case SFX_MENU_MOVE:    
            Beep(1800, 5); // Modern ui sound
            break;
        case SFX_MENU_KEY_TYPING:    
            Beep(1200, 10); // Tiny, instantaneous mechanical click
            break;
        case SFX_MENU_SUCCESS:       
            Beep(523, 100); Beep(659, 100); Beep(784, 100); Beep(1046, 200); // Ascending major chord (Login/Register success)
            break;
        case SFX_MENU_INVALID:       
            Beep(150, 100); // Single low thud
            break;
        case SFX_MENU_BLOCKED:       
            Beep(100, 50); Beep(100, 50); // Double low thud (hitting a wall)
            break;
        case SFX_MENU_ESCAPE:        
            Beep(400, 50); Beep(300, 80); // Descending blip (backing out)
            break;
        case SFX_MENU_CONTINUE:      
            Beep(800, 50); Beep(1200, 50); // Forward moving double-chirp
            break;

        // --- GAMEPLAY ---
        case SFX_START:         
            Beep(523, 100); Beep(784, 100); Beep(1046, 250); 
            break;
        case SFX_NORMAL_CORRECT:
            Beep(1046, 80); Beep(1318, 120); // Classic coin
            break;
        case SFX_QUICK_CORRECT: 
            Beep(1318, 50); Beep(1568, 50); Beep(2093, 150); // Faster, higher-pitched coin
            break;
        case SFX_GODLIKE_CORRECT:
            Beep(1046, 40); Beep(1318, 40); Beep(1568, 40); Beep(2093, 40); Beep(2637, 200); // 5-note sparkle
            break;
        case SFX_STREAK_BURST:  
            Beep(784, 40); Beep(1046, 40); Beep(1318, 100); // Quick 3-tone power burst
            break;
        case SFX_WRONG:         
            Beep(250, 150); Beep(150, 300); // Descending buzz
            break;
        case SFX_BOSS_ALARM: 
            Beep(300, 350); Beep(300, 300); Beep(300, 350); Beep(300, 300);
            break;
        case SFX_BOSS_DEFEAT:   
            Beep(1046, 100); Beep(784, 100); Beep(523, 100); Beep(784, 100); Beep(1046, 100); Beep(1567, 400); 
            break;
        case SFX_GAMEOVER:      
            Beep(622, 150); Beep(587, 150); Beep(554, 150); Beep(523, 500); 
            break;
        case SFX_SCORE_TICK:    
            Beep(2000, 15); // Extremely fast, high-pitched tick for the counter animation
            break;
        case SFX_SCORE_FINAL:   
            Beep(1046, 100); Beep(1568, 300); // The "Ta-Da!" when the score reveal finishes
            break;
        case SFX_HIGHSCORE:
            // Triumphant rapid fanfare (C, C, C, Ab, Bb, C)
            Beep(523, 100); Beep(523, 100); Beep(523, 100); 
            Beep(415, 150); Beep(466, 150); Beep(523, 400); 
            break;
    }
    
    return 0; // Thread naturally dies when the sound is done
}

void playAudio(SoundID id)
{
    HANDLE hThread = CreateThread(NULL, 0, SoundThread, (LPVOID)(intptr_t)id, 0, NULL);

    if (hThread != NULL) {
        CloseHandle(hThread); 
    }
}