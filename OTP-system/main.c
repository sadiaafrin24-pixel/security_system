#include <stdio.h>
#include <string.h>
#include<time.h>
#include <conio.h>

void menu();
void registerUser();
void login();
int generateOTP();
int verifyOTP(int generatedOTP);
int usernameExists(char username[]);
void inputPassword(char password[]);

int main()
{
    int choice;

    while (1)
    {
        menu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registerUser();
            break;

        case 2:
            login();
            break;

        case 3:
            printf("\nThank you for using the system.\n");
            return 0;

        default:
            printf("\nInvalid choice! Please try again.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();   // removes leftover newline
        getchar();   // waits for Enter
    }

    return 0;
}

void menu()
{
    printf("\n=====================================\n");
    printf("   OTP TOKEN AUTHENTICATION SYSTEM\n");
    printf("=====================================\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("=====================================\n");
}

int usernameExists(char username[])
{
    FILE *fp;
    printf("Checking existing usernames...\n");
    char storedUsername[50];
    char storedPassword[50];

    fp = fopen("users.txt", "r");

    if(fp == NULL)
        return 0;

    while (fscanf(fp, "%s %s", storedUsername, storedPassword) != EOF)
    {
    printf("Comparing '%s' with '%s'\n", username, storedUsername);

    if (strcmp(username, storedUsername) == 0)
    {
        printf("MATCH FOUND!\n");
        fclose(fp);
        return 1;
    }
    }

    fclose(fp);

    return 0;
}

void inputPassword(char password[])
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = getch();

        if (ch == 13)   // Enter key
        {
            break;
        }

        if (ch == 8)    // Backspace
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i] = ch;
            i++;
            printf("*");
        }
    }

    password[i] = '\0';
    printf("\n");
}

void registerUser()
{
    FILE *fp;

    char username[50];
    char password[50];

    fp = fopen("users.txt", "a");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("\n===== User Registration =====\n");

    printf("Enter Username: ");
    scanf("%s", username);

    if(usernameExists(username))
    {
        printf("\nUsername already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Password: ");
    inputPassword(password);

    fprintf(fp, "%s %s\n", username, password);

    fclose(fp);

    printf("\nRegistration Successful!\n");
}

void saveLog(char username[], char status[])
{
    FILE *fp;
    time_t now;

    fp = fopen("logs.txt", "a");

    if(fp == NULL)
    {
        printf("Unable to open log file.\n");
        return;
    }

    time(&now);

    fprintf(fp, "Username: %s\n", username);
    fprintf(fp, "Status: %s\n", status);
    fprintf(fp, "Time: %s", ctime(&now));
    fprintf(fp, "\n");

    fclose(fp);
}

void login()
{
    FILE *fp;

    char username[50];
    char password[50];

    char storedUsername[50];
    char storedPassword[50];

    int found = 0;

    fp = fopen("users.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("\n===== User Login =====\n");

    printf("Enter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    inputPassword(password);

    while (fscanf(fp, "%s %s", storedUsername, storedPassword) != EOF)
    {
        if (strcmp(username, storedUsername) == 0 &&
            strcmp(password, storedPassword) == 0)
        {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
    {
    int otp;
    
    printf("\nLogin Successful!\n");

    otp = generateOTP();
    
    printf("OTP has been generated.\n");
    printf("(For demonstration purposes, the OTP is displayed below.)\n");
    printf("OTP: %06d\n", otp);

    if (verifyOTP(otp))
    {
        saveLog(username, "SUCCESS");
        printf("\nAccess Granted!\n");
    }
    else
    {
        saveLog(username, "FAILED (OTP)");
        printf("\nAccess Denied!\n");
    }

    }
    else
    {
        saveLog(username, "FAILED (PASSWORD)");
        printf("\nInvalid Username or Password!\n");
    }
}

int generateOTP()
{
    time_t currentTime;

    currentTime = time(NULL);

    int otp = ((currentTime / 30) * 7919) % 1000000;

    if (otp < 0)
    {
        otp = -otp;
    }

    return otp;
}

int verifyOTP(int generatedOTP)
{
    int userOTP;
    int attempts = 3;

    while (attempts > 0)
    {
        printf("\nEnter OTP: ");
        scanf("%d", &userOTP);

        if (userOTP == generatedOTP)
        {
            printf("\nOTP Verified!\n");
            return 1;
        }

        attempts--;

        if (attempts > 0)
        {
            printf("\nIncorrect OTP!\n");
            printf("Attempts Left: %d\n", attempts);
        }
    }

    printf("\nToo many incorrect attempts.\n");

    return 0;
}