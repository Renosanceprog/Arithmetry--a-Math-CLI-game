//
#include <stdio.h>
#include <string.h>
#include <conio.h>

#define MAX_ACCOUNT 50
#define MAX_USERLEN 50
#define MAX_PASSLEN 20

int enterInteger(void);
int showMenu();
int loginUser();
int registerUser();

void hidePassword(char *password);
void saveData();
void loadData();


typedef struct
{
    int id;
    char userNames[MAX_USERLEN];
    char userPasswords[MAX_PASSLEN];
}loginInfo;

loginInfo userDB[MAX_ACCOUNT];
int activeUserIndex = -1;

int userCount = -1;

int main(){
    loadData();
    while (1)
    {
        switch (showMenu())
        {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
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
    printf("users loaded = %d\n[1] Register\n[2] Login\n[3] Exit\nChoose Option: ", userCount+1);
    int choice = enterInteger();
    return choice;
}

int loginUser()
{
    if (userCount == -1)
    {
        return -1;
    }
    
    int valid = 0;
    int index = 0;
    // username
    char tempName[MAX_USERLEN];
    do
    {
        index = 0;
        if (valid != 0) printf("\33[1A\33[2K\rUsername not Found! ");
        printf("Enter username: ");
        scanf(" %49[^\n]", tempName);
        while (index <= userCount)
        {
            valid = strcmp(userDB[index].userNames, tempName);
            if (valid == 0)
            {
                break;
            }
            index++;
        }
    } while (valid != 0);
    char tempPass[MAX_PASSLEN];
    do
    {
        if (valid != 0) printf("\33[1A\33[2K\rIncorrect Password! ");
        printf("Enter Password: ");
        hidePassword(tempPass);
            valid = strcmp(userDB[index].userPasswords, tempPass);
            if (valid == 0)
            {
                break;
            }
    } while (valid != 0);
    activeUserIndex = index;
    printf("%s succesfully logged in!\n", userDB[index].userNames);
    return 0;
}

int registerUser()
{
    if (userCount >= MAX_ACCOUNT - 1) { printf("Database full!\n"); return 1; }
    // username
    userCount++;
    printf("Enter username: ");
    scanf(" %49[^\n]", userDB[userCount].userNames);

    //password
    char password1[MAX_PASSLEN], password2[MAX_PASSLEN];
    int valid = 0;
    printf("Enter password (min 5 chars, no spaces): ");
    hidePassword(password1);
    printf("Enter password again: ");
    do
    {
        if (valid != 0)
        {
            printf("\33[1A\33[2K\rPassword incorrect, try again: ");
        }
        hidePassword(password2);
        valid = strcmp(password1,password2);
    } while (valid != 0);

    strcpy(userDB[userCount].userPasswords, password2);
    userDB[userCount].id = userCount;

    printf("%s succesfully Registered!\n", userDB[userCount].userNames);
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
            if (i <= 4) // Enforce minimum length (5 chars)
            {
                continue; 
            }
            break; // Submit the password
        }
        else if (c == 8 || c == 127)
        {
            if (i > 0)
            {
                printf("\b \b");
                i--;
            }
        }
        else if (c == 32) 
        {
            continue;
        }
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
    FILE *pData = fopen("programdata.txt", "w");
    if (userCount == -1) { printf("Database is empty. Nothing to save.\n"); return;}
    if(pData == NULL){ printf("Error opening file\n"); return;}
    for (int i = 0; i <= userCount; i++)
    {
        fprintf(pData, "%d,%s,%s\n", userDB[i].id, userDB[i].userNames, userDB[i].userPasswords);
    }
    fclose(pData);
}

void loadData()
{
    userCount = -1;
    int id;
    char username[MAX_USERLEN];
    char password[MAX_PASSLEN];
    
    FILE *pData = fopen("programdata.txt", "r");
    if(pData == NULL){ printf("Error opening file\n"); return;}

    while (fscanf(pData, "%d,%[^,],%s\n", &id, username, password) == 3)
    {
        if (userCount >= MAX_ACCOUNT - 1)
        {
            break;
        }
        userCount++;
        userDB[userCount].id = id;
        strcpy(userDB[userCount].userNames, username);
        strcpy(userDB[userCount].userPasswords, password);
    }
    fclose(pData);
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