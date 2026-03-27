//
#include <stdio.h>
#include <string.h>
#include <conio.h>

#define MAX_ACCOUNT 50
#define MAX_USERLEN 50
#define MAX_PASSLEN 20

typedef struct
{
    int Classic;
    int Addition;
    int Subtraction;
    int Multiplication;
    int Division;
}Scores;


typedef struct
{
    int id;
    char userNames[MAX_USERLEN];
    char userPasswords[MAX_PASSLEN];
    Scores userScores;
}loginInfo;

int enterInteger(void);
int showMenu();
int showCategoryMenu();
void showUserStats();
int getScore(loginInfo user, int category);
void showLeaderboard(int category);
void saveData();
void loadData();

loginInfo userDB[MAX_ACCOUNT];

int curID = 3;

int userCount = -1;

int main(){
    loadData();
    while (1)
    {
        switch (showMenu())
        {
        case 1:
            showCategoryMenu();
            break;
        case 2:
            showUserStats();
            break;
        case 3:
            showLeaderboard(showCategoryMenu());
            break;
        case 4:
            saveData();
            return 0;
            break;
        default:
            break;
        }
    }
}


int showMenu()
{
    printf("[1] Play\n[2] Show Stats\n[3] Show leaderboard\n[4] Exit\nChoose Option: ");
    int choice = enterInteger();
    return choice;
}

int showCategoryMenu()
{
    printf("[1] Classic\n[2] Addition Only\n[3] Subtraction Only\n[4] Multiplication Only\n[5] Division Only\n[6] Overall\n[7] Exit\nChoose Option: ");
    int choice = enterInteger();
    return choice;
}

void showUserStats()
{
    printf("==== USER STATS ====\n");
    printf("ID: %02d\n",userDB[curID].id);
    printf("Username: %s\n\n",userDB[curID].userNames);
    int overallScore = userDB[curID].userScores.Classic + userDB[curID].userScores.Addition + userDB[curID].userScores.Subtraction + userDB[curID].userScores.Multiplication + userDB[curID].userScores.Division;
    printf("    -=(Top Scores)=-\n%-16s | %5d\n%-16s | %5d\n%-16s | %5d\n%-16s | %5d\n%-16s | %5d\n%-16s | %5d\n", 
        "Classic",userDB[curID].userScores.Classic,
        "Addition",userDB[curID].userScores.Addition,
        "Subtraction",userDB[curID].userScores.Subtraction,
        "Multiplication",userDB[curID].userScores.Multiplication,
        "Division",userDB[curID].userScores.Division, 
        "Overall",overallScore);
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

void showLeaderboard(int category) {
    if (userCount < 0) {
        printf("No users in database to show.\n");
        return;
    }

    // 1. Create a temporary copy (so we don't mess up the real IDs)
    loginInfo tempDB[MAX_ACCOUNT];
    int count = userCount + 1;
    for (int i = 0; i < count; i++) {
        tempDB[i] = userDB[i];
    }

    // 2. Good old-fashioned Bubble Sort!
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            
            // Grab the scores we are comparing
            int scoreA = getScore(tempDB[j], category);
            int scoreB = getScore(tempDB[j + 1], category);

            // If the person below has a higher score, swap them!
            if (scoreB > scoreA) { 
                loginInfo temp = tempDB[j];
                tempDB[j] = tempDB[j + 1];
                tempDB[j + 1] = temp;
            }
        }
    }

    // 3. Print the formatted table
    char *titles[] = {"", "CLASSIC", "ADDITION", "SUBTRACTION", "MULTIPLICATION", "DIVISION", "OVERALL"};
    
    printf("\n--- %s LEADERBOARD ---\n", titles[category]);
    printf("%-4s %-20s %-10s\n", "RANK", "USERNAME", "SCORE");
    printf("------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-4d %-20s %-10d\n", i + 1, tempDB[i].userNames, getScore(tempDB[i], category));
    }
    printf("\n");
}

void saveData()
{
    // Check BEFORE opening the file to prevent accidental deletion
    if (userCount == -1) { 
        printf("Database is empty. Nothing to save.\n"); 
        return;
    }

    FILE *pData = fopen("programdata.txt", "w");
    if(pData == NULL){ 
        printf("Error opening file\n"); 
        return;
    }

    for (int i = 0; i <= userCount; i++)
    {
        // Added the 5 score variables to the end of the line
        fprintf(pData, "%d,%s,%s,%d,%d,%d,%d,%d\n", 
                userDB[i].id, 
                userDB[i].userNames, 
                userDB[i].userPasswords,
                userDB[i].userScores.Classic,
                userDB[i].userScores.Addition,
                userDB[i].userScores.Subtraction,
                userDB[i].userScores.Multiplication,
                userDB[i].userScores.Division);
    }
    fclose(pData);
    printf("Data saved successfully.\n");
}

void loadData()
{
    userCount = -1;
    // Temporary variables to hold the scores during reading
    int id, s1, s2, s3, s4, s5;
    char username[MAX_USERLEN];
    char password[MAX_PASSLEN];
    
    FILE *pData = fopen("programdata.txt", "r");
    if(pData == NULL){ 
        printf("No existing database found. Starting fresh.\n"); 
        return;
    }

    // The format string now expects 8 items (3 strings/id + 5 scores)
    // We use %[^,] for the password too, just in case.
    while (fscanf(pData, "%d,%[^,],%[^,],%d,%d,%d,%d,%d\n", 
                &id, username, password, &s1, &s2, &s3, &s4, &s5) == 8)
    {
        if (userCount >= MAX_ACCOUNT - 1) break;

        userCount++;
        userDB[userCount].id = id;
        strcpy(userDB[userCount].userNames, username);
        strcpy(userDB[userCount].userPasswords, password);
        
        // Assigning the loaded scores to the nested struct
        userDB[userCount].userScores.Classic = s1;
        userDB[userCount].userScores.Addition = s2;
        userDB[userCount].userScores.Subtraction = s3;
        userDB[userCount].userScores.Multiplication = s4;
        userDB[userCount].userScores.Division = s5;
    }
    fclose(pData);
}

// void saveData()
// {
//     // Open in "Write Binary" mode
//     FILE *pData = fopen("programdata.dat", "wb");
//     if (pData == NULL) { printf("Error opening file\n"); return; }

//     // 1. Save the total number of users first, so we know how many to load later
//     fwrite(&userCount, sizeof(int), 1, pData);

//     // 2. Save the ENTIRE userDB array in one single line!
//     if (userCount >= 0)
//     {
//         // params: (Pointer to data, Size of one item, How many items, File pointer)
//         fwrite(userDB, sizeof(loginInfo), userCount + 1, pData);
//     }

//     fclose(pData);
//     printf("Data saved successfully!\n");
// }

// void loadData()
// {
//     // Open in "Read Binary" mode
//     FILE *pData = fopen("programdata.dat", "rb");
//     if (pData == NULL) 
//     { 
//         // If file doesn't exist, start with an empty database
//         userCount = -1; 
//         return; 
//     }

//     // 1. Read the userCount so we know how big the database is
//     fread(&userCount, sizeof(int), 1, pData);

//     // 2. Read the ENTIRE array back into memory in one line!
//     if (userCount >= 0)
//     {
//         fread(userDB, sizeof(loginInfo), userCount + 1, pData);
//     }

//     fclose(pData);
// }

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